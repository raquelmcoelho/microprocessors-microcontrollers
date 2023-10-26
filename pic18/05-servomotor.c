#include <p18f4520.h>

#define serv_pan LATCbits.LATC0
#define serv_tilt LATCbits.LATC3
#define pan_up PORTCbits.RC1
#define pan_down PORTCbits.RC2
#define tilt_up PORTCbits.RC4
#define tilt_down PORTCbits.RC5

void configura_sistema(void);
void delay_grau(void);
void servo_pan(unsigned char);
void servo_tilt(unsigned char);
void checar_pan(void);
void checar_tilt(void);
void delay_ms(int);
int ang1 = 30;
int ang2 = 30;

void delay_ms(int ms)

{
	unsigned int i;
	for (; ms > 0; ms--)
	{
		for (i = 0; i < 529; i++)
		{
		} // tempo de loop = 1ms
	}
}

void delay_grau()
{
	unsigned char i;
	for (i = 0; i < 2; i++)
	{
	}
	i = 0;
	i = 0;
	i = 0;
	i = 0;
	i = 0;
	i = 0;
}

void servo_tilt(unsigned char ang_tilt)
{
	unsigned char aux = ang_tilt;
	aux = ang_tilt / 11;
	serv_tilt = 1;
	delay_ms(1);
	for (; ang_tilt > 0; ang_tilt--)
	{
		delay_grau();
	}
	for (; aux > 0; aux--)
	{
	};
	serv_tilt = 0;
}

void servo_pan(unsigned char ang_pan)
{
	unsigned char aux = ang_pan;
	aux = ang_pan / 11;
	serv_pan = 1;
	delay_ms(1);
	for (; ang_pan > 0; ang_pan--)
	{
		delay_grau();
	}
	for (; aux > 0; aux--)
	{
	};
	serv_pan = 0;
}

void checar_pan(void)
{
	if (pan_up == 1)
		ang1++;
	if (pan_down == 1)
		ang1--;
	while (pan_up == 1 || pan_down == 1)
	{
	}
}

void checar_tilt(void)
{
	if (tilt_up == 1)
		ang2++;
	if (tilt_down == 1)
		ang2--;
	while (tilt_up == 1 || tilt_down == 1)
	{
	}
}

void main(void)

{
	TRISCbits.TRISC0 = 0;
	TRISCbits.TRISC3 = 0;
	TRISCbits.TRISC1 = 1;
	TRISCbits.TRISC2 = 1;
	TRISCbits.TRISC4 = 1;
	TRISCbits.TRISC5 = 1;

	while (1)
	{
		servo_pan(ang1);
		servo_tilt(ang2);
		delay_ms(20);
		checar_pan();
		checar_tilt();
	}
}