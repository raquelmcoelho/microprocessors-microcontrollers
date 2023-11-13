#include <p18f4520.h>


#define led PORTCbits.RC7


void Configuracao_do_Pic ()
{

// ------ configuração do timer 0, modo 16 bits, tempo de 1 segundo

	T0CONbits.TMR0ON=1;		//LIGA TIMER0
	T0CONbits.T08BIT=0;		//16 bits
	T0CONbits.T0CS=0;		//OSCILADOR INTERNO (timer)
	T0CONbits.PSA=0;		//COM PRESCALER
	T0CONbits.T0PS=4;
//	T0CONbits.T0PS1=0;
//	T0CONbits.T0PS0=0;		//PRESCALER 1:32
	TMR0H=0X0b;
	TMR0L=0Xdc;				//65536-62500 = 3036 -> 1 seg

// 0000 1011 1101 1100 = 4+8+16+64+128+256+512+2048=3036

// 8MHz/4=> 2MHz/32=> 62500Hz -> 65536-62500=3036


//----- configura a interrupção do timer 0

	INTCONbits.TMR0IF=0;
	INTCONbits.TMR0IE=1;
	INTCON2bits.TMR0IP=1;	
	INTCONbits.GIE=1;		//HABILITO O SISTEMA DE INTERRUPÇAO DE ALTA PRIORIDADE
	

//Configuração da direção dos pinos de I/O

TRISCbits.RC7=0; //lampada como saida
ADCON1 = 0B00001111; //configura todos os pinos das portas como I/O


//TRISC = 0b01111111; // configura LED como saida (RC7=0), INTERRUPTOR como entrada (RC0=1)
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
//		Odometro++;				//registra volta da roda
//		Tempo_roda = Timer0;	//registra tempo da volta da roda
//		Calcula_Velocidade (Tempo_roda);
//		Timer0=0;				//prepara timer0 para a próxima volta da roda		
		INTCONbits.INT0IF=0;
	}
	if (INTCONbits.TMR0IF)
	{
		TMR0H=0X0b;
		TMR0L=0Xdc;				//65536-62500 = 3036 -> 1 seg
		led=~led;
		INTCONbits.TMR0IF=0;
	}
}

//____________________________________________

void main()
{

Configuracao_do_Pic ();

	while(1)
	{
	}
}


