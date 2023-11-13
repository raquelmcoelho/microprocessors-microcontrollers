#include <p18f4520.h>

int tensao;

void Configuracao_do_Pic ()
{
	ADCON0bits.CHS3=0;
	ADCON0bits.CHS2=0;
	ADCON0bits.CHS1=0;
	ADCON0bits.CHS0=0;		//SELECIONA AN0

	ADCON0bits.ADON=1;		//LIGA O CONVERSOR	

	ADCON1bits.PCFG3=1;
	ADCON1bits.PCFG2=1;
	ADCON1bits.PCFG1=1;
	ADCON1bits.PCFG0=0;		//USA SOMENTE AN0

	ADCON1bits.VCFG1=0;		//VREF- (GND)
	ADCON1bits.VCFG0=0;		//VREF+ (5V)
	
	ADCON2=0B10111111;		//justificado pela direita
}

void tratamento_int_alta (void);


//____________________________________________

#pragma code vetor_alta=0x0008

void int_alta_prioridade (void)

{
	_asm GOTO tratamento_int_alta _endasm		
}

#pragma code

//____________________________________________

#pragma interrupt tratamento_int_alta

void tratamento_int_alta (void)

{
	if (INTCONbits.INT0IF)
	{
		INTCONbits.INT0IF=0;
	}

	if (INTCONbits.TMR0IF)
	{
		INTCONbits.TMR0IF=0;
	}
}

//____________________________________________

void main()
{

Configuracao_do_Pic ();

	while(1)
	{
		ADCON0bits.GO=1;			//INICIA UMA CONVERSÃO
		while (ADCON0bits.GO) {};	//ESPERA TERMINAR A CONVERSÃO
		tensao=(ADRESH*256)+ADRESL;	
		tensao=tensao*(500.0/1024.0);
	}
}


