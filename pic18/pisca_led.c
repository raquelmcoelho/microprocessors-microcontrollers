#include <p18f4520.h>
#include <delays.h>

#define led	LATCbits.LATC0

void	configura_sistema(void);
void 	delay_us (unsigned int);
void 	delay_ms (unsigned int);
void 	delay_s (unsigned int);


void	configura_sistema(void)

{
	TRISCbits.TRISC0=0;	//CONFIGURA LED COMO SAIDA
}


void delay_s(unsigned int s)
{
	unsigned int i;
	for (;s>0;s--)
	{
		delay_ms(1000);		//delay de 1 seg]
	}
}

void delay_ms(unsigned int ms)

{
	int i,j;
	for (;ms>0;ms--)
	{
	for (i=0;i<109;i++) 
	{
	}		//tempo de loop = 1ms
	i=0;
	i=0;
	}	

}


void delay_us(unsigned int us)	//funciona a partir de 10us
{

}


main ()

{
	configura_sistema();

while (1)

	{
		led=1;
		delay_ms(1000);
		led=0;
		delay_ms(1000);
	}
}
