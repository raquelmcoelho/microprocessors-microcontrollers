#include <p18f4520.h>

#define	pin_serv       LATCbits.LATC0

void configura_sistema(void);
void delay_grau(void);
void servo (unsigned char);
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
	for (i=0;i<529;i++) {}		//tempo de loop = 1ms
	}
}

void delay_us(int us)	//funciona a partir de 10us
{
	unsigned char i;
	for (i=0;i<28;i++) {}
}

void delay_grau() 
{
    unsigned char i;
    for (i=0;i<3;i++) {}
}

void servo (unsigned char ang)
{
   pin_serv = 1;
   delay_ms(1);
   for (;ang>0;ang--) 
   {
    delay_grau ();
   }
   pin_serv = 0;
  

}


void main (void)

{
   TRISCbits.TRISC0 = 0;
   while(1)
  {
    servo(177);
    delay_ms(20);
  }

}