#include <p18f4520.h>
#define led PORTCbits.RC6

void tratamento_int_alta (void);

//____________________________________________

#pragma code vetor_alta=0x0008 // endereço onde fica instruções para interrupt de high priority

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
		led=~led;                                  // pisca led
		INTCON2bits.INTEDG0=~INTCON2bits.INTEDG0;  // muda estado bool
		INTCONbits.INT0IF=0;                       // rearma a int0
	}
}

void configuracao_do_pic (void)
{
	INTCONbits.INT0IF=0;	//arma a int0
	INTCONbits.INT0IE=1;	//habilita int0
	INTCON2bits.INTEDG0=0;	//inicia como borda de subida
	INTCONbits.GIE=1;		//HABILITO O SISTEMA DE INTERRUP�AO DE ALTA PRIORIDADE
	
	//Configura��o da dire��o dos pinos de I/O

	TRISBbits.RB0=1;        //interruptor como entrada
	TRISCbits.RC6=0;        //lampada como saida
	ADCON1 = 0B00001111;    //configura todos os pinos das portas como I/O
}

//____________________________________________

void main()
{

	configuracao_do_pic ();

	while(1)
	{
	}
}

