#include <p18f4520.h>

#define display_bcd 	LATC	
#define sinal			PORTBbits.RB0

int tempo_hi, tempo_low, periodo;


void configura_sistema(void);
void delay_ms (int);
void delay_s (int);
void delay_us (int); //não resolvido
void mede_periodo(void);
void calcula_frequencia(void);
void mostra_frequencia(void);


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
//	i=0;
}

void mede_periodo(void)

{
	tempo_hi=0;
	tempo_low=0;
	periodo=0;

	while(sinal) {};
	while(!sinal) {};	//detecta a borda de subida do sinal (0 -> 1)

	while(sinal)

		{
			tempo_hi++;
			delay_us(100);		//para um erro de 1% em 100Hz
			TRISBbits.TRISB0=1;	//sinal de frequencia configurado como entrada
		}

	while(!sinal)

		{
			tempo_low++;
			delay_us(100);		//para um erro de 1% em 100Hz
			TRISBbits.TRISB0=1;	//sinal de frequencia configurado como entrada
		}

	periodo=tempo_hi+tempo_low;
}

void calcula_frequencia(void)

{

}


void mostra_frequencia(void)

{

}


void frequencimetro(void)

{
		mede_periodo ();
		calcula_frequencia ();
		mostra_frequencia ();
}

void configura_sistema (void)
{
	ADCON1=0B00001111;	//DESABILITA ANALOGICOS
	TRISC=0B00000000;	//display como saida
	TRISBbits.TRISB0=1;	//sinal de frequencia configurado como entrada
}


main ()

{
	configura_sistema ();

	while (1)

	{
		frequencimetro();
	}
}