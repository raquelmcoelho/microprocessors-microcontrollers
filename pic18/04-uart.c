#include <p18f4520.h>

#define Tx   PORTCbits.RC0
#define Rx   PORTCbits.RC1

char caractere = 0x00;
void delay_1200bps(void);
void envia_serial();
char recebe_serial();


void delay_1200bps(void)
{
	unsigned int i;
	for (i=0;i<109;i++) {}		//tempo de loop = 833us
	// for de 833us (1/1200)
}

void envia_serial()//A=01000001
{
	int i;
	Tx=1;				//repouso
	delay_1200bps();
	Tx=0;				//start
	delay_1200bps();


    for (i = 0; i <= 7;i++){
        Tx=caractere & 1;
        delay_1200bps();	
        caractere=caractere>>1;
    }
	delay_1200bps();
	
}

char recebe_serial ()

{
	int i;
	char auxiliar;
	delay_1200bps();
	
	while(Rx);
	
	delay_1200bps();
	
	for(i = 0; i < 7; i++)
	{
		auxiliar = Rx<<7;
		caractere = caractere | auxiliar;
		caractere=caractere>>1;
		delay_1200bps();
	}
}

void main ()

{	
	TRISCbits.RC0 = 0;
    TRISCbits.RC1 = 1;
	
	while (1)
	{
        caractere = 0B00000000;
        recebe_serial();
		envia_serial();
    }
}

