#include <p18f4520.h>

#define	uni_7seg		LATB
#define dez_7seg		LATD
#define bcd 			LATC	
#define botao			PORTAbits.RA0

char converte_7seg [] = 
{
//hgfedcba		//mapeamento de segmentos do display 7seg
0b00111111,		//"0"
0b00000110,		//"1"
0b01011011,		//"2"
0b01001111,		//"3"
0b01100110,		//"4"
0b01101101,		//"5"
0b01111101,		//"6"
0b00000111,		//"7"
0b01111111,		//"8"
0b01101111,		//"9"
};

char unidade = 0;
char dezena = 0;

void configura_sistema(void);
void delay_ms (int);
void delay_s (int);
void delay_us (int); //não resolvido

void delay_s (int s)
{
	unsigned long int i;
	for (;s>0;s--)
	{
		for (i=0;i<83333;i++) {};
	}
}


void delay_ms(int ms)

{
	unsigned int i;
	for (;ms>0;ms--)
	{
	for (i=0;i<131;i++) {}		//tempo de loop = 1ms
	}
}

void delay_us(int us)	//funciona a partir de 10us
{
	unsigned char i;
	for (i=0;i<28;i++) {}
}



void mostra_7seg(void)
{
	uni_7seg=converte_7seg[unidade];		
	dez_7seg=converte_7seg[dezena];	
}




void mostra_bcd(void)
{
/*
	dezena=2 (00000010), unidade=5 (00000101)
	
	dezena  = 00100000  << 4
	unidade = 00000101
	   OU   = 00100101

*/	
	bcd=((dezena<<4)|(unidade));
}

void mostra_display(char display)

{
		if (botao)
		{
			mostra_bcd ();
			TRISC=0B00000000;
			TRISD=0B11111111;
			TRISB=0B11111111;
		}
		else
		{
			mostra_7seg();
			TRISC=0B11111111;
			TRISD=0B00000000;
			TRISB=0B10000000;
		}

}

void relogio(void)
{
		if (unidade++>=9)
		{
			unidade=0;
			if (dezena++>=5)
			{
				dezena=0;
			}		
		}
}

void configura_sistema (void)
{
		//  76543210
	TRISB=0B00000000;	//uni_7seg
	TRISD=0B00000000;	//dez_7seg
	TRISC=0B00000000;	//BCD
	TRISAbits.TRISA0=1;	//botao (input)
	ADCON1=0B00001111;	//DESABILITA ANALOGICOS
}


main ()

{
	configura_sistema ();

	while (1)

	{
		mostra_display (100);
		delay_s(1);			//temporiza o relogio
		relogio();
	}
}