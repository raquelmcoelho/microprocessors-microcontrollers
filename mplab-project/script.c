#include <p18f4520.h>


#define led PORTCbits.RC6

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
		led=~led;
		INTCON2bits.INTEDG0=~INTCON2bits.INTEDG0;
		INTCONbits.INT0IF=0;
	}
}

void Configuracao_do_Pic (void)
{
	INTCONbits.INT0IF=0;	//arma a int0
	INTCONbits.INT0IE=1;	//habilita int0
	INTCON2bits.INTEDG0=0;	//inicia como borda de subida
	INTCONbits.GIE=1;		//HABILITO O SISTEMA DE INTERRUPÇAO DE ALTA PRIORIDADE
	
//Configuração da direção dos pinos de I/O

TRISBbits.RB0=1; //interruptor como entrada
TRISCbits.RC6=0; //lampada como saida
ADCON1 = 0B00001111; //configura todos os pinos das portas como I/O


}

//____________________________________________

void main()
{

Configuracao_do_Pic ();

	while(1)
	{
	}
}

