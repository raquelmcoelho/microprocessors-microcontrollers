#include <p18f4520.h>

#define RIGHTDISPLAY PORTB
#define LEFTDISPLAY PORTC
#define COLUMNS PORTD
#define MATRIXSIZE 8

unsigned int *back, *front;
unsigned int message[] = {
	0b11111111, //<-back
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111, //<-front
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,

	0b00000001, // R
	0b11110101,
	0b11010101,
	0b10111011,
	0b01111111,
	0b11111111, // espaço

	0b00000011, // A
	0b11101101,
	0b11101101,
	0b00000001,
	0b11111111, // espaço
	0b11111111, // espaço

	0b11111111, // Q
	0b10000011,
	0b01111101,
	0b01101101,
	0b10000001,
	0b10111111, // espaço

	0b10000001, // U
	0b01111111,
	0b01111111,
	0b10000001,
	0b11111111, // espaço
	0b11111111, // espaço

	0b00000001, // E
	0b01101101,
	0b01101101,
	0b01101101,
	0b11111111, // espaço
	0b11111111, // espaço

	0b00000001, // L
	0b01111111,
	0b01111111,
	0b01111111,
	0b11111111, // espaço
	0b11111111, // espaço

	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
	0b11111111,
};

void configure();
void delay_scan(void)
{
	unsigned int i;
	for (i = 0; i < 830; i++)
		;
}

void show_message(void)
{
	unsigned int aux = 1;
	RIGHTDISPLAY = 0b11111111; // right
	LEFTDISPLAY = 0b11111111;  // left
	back = &message[0];
	front = &message[MATRIXSIZE];
	while (front < &message[67])
	{ // front will wait back to reach all the list members
		if (aux > MATRIXSIZE)
		{ // when finished to show message shows again with offset of 1 position
			static const int offset = 1;
			front -= (MATRIXSIZE - offset);
			back -= (MATRIXSIZE - offset);
			aux = 1;
		}
		LEFTDISPLAY = *back;
		RIGHTDISPLAY = *front;
		front += 1;
		back += 1;
		delay_scan();

		if (COLUMNS == 0b00000000)
			COLUMNS = 0b00000001;
		else
		{
			COLUMNS = COLUMNS << 1;
			aux++;
		}
	}
}

void configure()
{
	TRISB = 0B00000000; // right DotMatrix
	TRISC = 0B00000000; // left DotMatrix
	TRISD = 0B00000000; // displays columns
	ADCON1 = 0b00001111;
}

void main(void)
{
	configure();
	while (1)
		show_message();
}