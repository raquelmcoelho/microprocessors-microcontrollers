/* Marcelo De Ara�jo */

#include <p18f4520.h>

#pragma config OSC = INTIO67

#define RS LATCbits.LATC0
#define RW LATCbits.LATC1
#define E LATCbits.LATC2

#define INTERRUPTOR PORTBbits.RB0
#define led PORTCbits.RC7

#define LCD LATD

/* ####=||=||=||=||=||=Prot�tipos=||=||=||=||=||=||=#### */

void interruptTreatment(void);
void delay_ms(int ms);
void delay_s(int s);
void sendCommandLCD(char command);
void initializeLCD(void);
void sendDataLCD(char data);
void sendLCDText(char text[]);
void clearDisplay(void);
void configurationPic(void);
void calculateSpeed(int meters, int seconds);
void sendLCDValue(unsigned int value);
void calculateSpeedRPM(int rotations, int seconds);

/* ####=||=||=||=||=||= Vari�veis =||=||=||=||=||=||=#### */

unsigned int odometer = 0;
unsigned int motorRotation = 0;
unsigned int totalDistance = 0;
unsigned int timer = 0;
unsigned int speed = 0;
unsigned int speedRPM = 0;

void delay_ms(int ms)
{
	unsigned int i = 0;
	for (; ms > 0; ms--)
	{
		for (i = 0; i < 131; i++)
		{
		}
	}
}

void delay_s(int s)
{
	int i = 0;
	for (; s > 0; s--)
	{
		delay_ms(1001); // 1 seg -  1001
		for (i = 0; i < 60; i++)
		{
		}
	}
}

void sendCommandLCD(char command)
{
	RS = 0;
	RW = 0;

	LCD = command;

	E = 1;
	E = 0;
	delay_ms(1);

	LCD = command << 4;

	E = 1;
	E = 0;

	delay_ms(1);
}

void initializeLCD()
{
	int i = 0;
	char commands[] = {0b0010,
					   0b00101000,
					   0b00001100,
					   0b00000001};
	delay_s(1);
	for (i = 0; i < 4; i++)
	{ // 4 = tamanho commands
		sendCommandLCD(commands[i]);
		delay_ms(30);
	}
}

void sendDataLCD(char data)
{
	RS = 1;
	RW = 0;

	LCD = data;

	E = 1;
	E = 0;
	delay_ms(1);

	LCD = data << 4;

	E = 1;
	E = 0;
	delay_ms(1);
}

void sendLCDText(char text[])
{
	int len = 0;
	int i = 0;

	while (text[len] != '\0')
		len++;

	for (i = 0; i < len; i++)
		sendDataLCD(text[i]);
}

void sendLCDValue(unsigned int value)
{
	char i;
	int dezena = 0;
	int unidade = 0;
	int centena = 0;
	int milhar = 0;

	if (value > 999)
	{
		milhar = (int)value / 1000;
		value = value % 1000;

		centena = (int)value / 100;
		value = value % 100;

		dezena = (int)value / 10;
		unidade = (int)value % 10;

		sendDataLCD(milhar + '0');
		sendDataLCD(centena + '0');
		sendDataLCD(dezena + '0');
		sendDataLCD(unidade + '0');
	}
	else if (value > 99)
	{
		centena = (int)value / 100;
		value = value % 100;
		dezena = (int)value / 10;
		unidade = value % 10;

		sendDataLCD(centena + '0');
		sendDataLCD(dezena + '0');
		sendDataLCD(unidade + '0');
	}
	else if (value > 9)
	{
		dezena = (int)value / 10;
		unidade = value % 10;

		sendDataLCD(dezena + '0');
		sendDataLCD(unidade + '0');
	}
	else
	{

		sendDataLCD(value + '0');
	}
}

void clearDisplay()
{
	sendCommandLCD(0b0000);
	sendCommandLCD(0b0001);
	delay_ms(50);
}

void calculateSpeed(int meters, int seconds)
{
	speed = (meters / seconds) * 3.6;
}

void calculateSpeedRPM(int rotations, int seconds)
{
	speedRPM = (rotations / seconds) * 60;
}

void configurationPic()
{
	T0CONbits.T08BIT = 0;
	T0CONbits.PSA = 0;
	T0CONbits.T0PS2 = 1;
	T0CONbits.T0PS1 = 1;
	T0CONbits.T0PS0 = 0;
	T0CONbits.T0CS = 0;
	T0CONbits.TMR0ON = 1;
	TMR0H = 0XC2;
	TMR0L = 0XF7;

	INTCON2bits.INTEDG0 = 1;

	INTCONbits.INT0IE = 1;
	INTCONbits.INT0IF = 0;

	INTCON3bits.INT2IP = 1;
	INTCON3bits.INT2IE = 1;
	INTCON3bits.INT2IF = 0;

	INTCONbits.GIE = 1;

	OSCCON = 0b11111111;
	ADCON1 = 0b00001111;

	TRISBbits.RB0 = 1;
	TRISC = 0B01110000;
	TRISD = 0B00001111;
}

//====================================================

#pragma code vetor_alta = 0x0008

void highPriorityInterrupt(void)
{
	_asm GOTO interruptTreatment _endasm
}

#pragma code

//====================================================

#pragma interrupt interruptTreatment

void interruptTreatment()
{
	if (INTCONbits.INT0IF)
	{
		odometer++; // Completou uma volta na roda.
					// 1 volta -> 1 metro.
		INTCONbits.INT0IF = 0;
	}
	if (INTCON3bits.INT2IF)
	{
		motorRotation++; // Pist�o subiu e desceu.
		INTCON3bits.INT2IF = 0;
	}
	if (INTCONbits.TMR0IF)
	{
		TMR0H = 0XC2;
		TMR0L = 0XF7; // Timer de 1 segundo.
		timer++;
		totalDistance = totalDistance + odometer;
		calculateSpeed(odometer, timer); // Recebe quantos metros se passou em um segundo
										 // e calcula em KM/H

		calculateSpeedRPM(motorRotation, timer); // Recebe quantas vezes o pist�o subiu e desceu
												 // e calcula a RPM.
		odometer = 0;							 // Zera o Odometro para a pr�xima volta.
		motorRotation = 0;						 // Zera o contador de rota��o do motor.
		timer = 0;								 // Zera o Timer para a pr�xima interrup��o
		INTCONbits.TMR0IF = 0;
	}
}

//=====================================================

void main()
{
	char kmhPhrase[] = "KM/H ";
	char speedPhrase[] = "V: ";
	char rpmPhrase[] = "RPM: ";
	char quilometerPhrase[] = "KM: ";
	configurationPic();
	initializeLCD();

	while (1)
	{
		sendCommandLCD(0x80);
		sendLCDValue(speed);
		sendLCDText(kmhPhrase);
		sendLCDText(rpmPhrase);
		sendLCDValue(speedRPM);
		sendCommandLCD(0xC0);
		sendLCDText(quilometerPhrase);
		sendLCDValue(totalDistance);
	}
}