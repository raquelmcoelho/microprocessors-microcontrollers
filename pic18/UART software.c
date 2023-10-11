#include <p18f4520.h>

#define Tx LATCbits.LATC0

void delay_ms (int);
void delay_s (int);
void delay_us (int); //não resolvido


void delay_s(int s)
{
	int i;
	for (;s>0;s--)
	{
		delay_ms(1001);		//delay de 1 seg]
		for (i=0;i<60;i++) {}	
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

}

void delay_1200bps(void)
{
	unsigned int i;
	for (i=0;i<109;i++) {}		//tempo de loop = 833us
	// for de 833us (1/1200)
}

void envia_serial(char caractere)//A=01000001

//	01000001 >>1
//  00100000 >>1
//  00010000 >>1
{
	Tx=1;				//repouso
	delay_1200bps();
	Tx=0;				//start
	delay_1200bps();

	Tx=caractere;				//A=0b01000001
	delay_1200bps();	
	caractere=caractere>>1;		//A=0b00100000
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;
	
	Tx=caractere;
	delay_1200bps();	
	caractere=caractere>>1;

	Tx=1;				//stop
	delay_1200bps();
	
}

char recebe_serial (void)

{
//	char auxiliar,caractere;
	
//	while (rx) {};
//	auxiliar=rx						//auxiliar=0b0000000B
//	auxiliar=auxiliar<<7			//auxiliar=0bB0000000
//	caractere=caractere|auxiliar	//
//	caractere=caractere>>1
}

main ()

{	//área de configuração (só roda uma vez)
	char i;
	TRISC=0B0000010;
	
	while (1)	//área de aplicação (roda em loop infinito)
	{
		envia_serial('A');
		envia_serial('n');
		envia_serial('a');
		envia_serial('x');
		envia_serial('a');
		envia_serial('g');
		envia_serial('o');
		envia_serial('r');
		envia_serial('a');
		envia_serial('s');
		envia_serial(' ');
		envia_serial(13);
		delay_s(1);
	}
}

