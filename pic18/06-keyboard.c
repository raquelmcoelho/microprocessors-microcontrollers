#include <p18f4520.h>

#define	SCAN_KEYBOARD_1COL   LATAbits.LATA0
#define	SCAN_KEYBOARD_2COL   LATAbits.LATA1
#define	SCAN_KEYBOARD_3COL   LATAbits.LATA2
#define	INPUT_1ROW		     PORTBbits.RB0
#define	INPUT_2ROW		     PORTBbits.RB1
#define	INPUT_3ROW		     PORTBbits.RB2
#define	INPUT_4ROW		     PORTBbits.RB3
#define CHARACTER            LATC	
#define SCAN_DISPLAY         LATD

unsigned int message[] = {
	0B00000000,
	0B00000000,
	0B00000000,
	0B00000000,
	0B00000000,
	0B00000000
};

void delay();
void configure ();
void add_char(char ch);
void check_row(int col);
void scan_input();
void scan_output();
void run();

void delay()
{
	unsigned int i;

	for (i=0;i<100;i++) {}		//tempo de loop = 1ms
}


void configure ()
{
		//  76543210	
    TRISC=0B00000000;	
	TRISD=0B00000000;
	ADCON1=0B00001111;
	TRISAbits.TRISA0 = 0;
	TRISAbits.TRISA1 = 0;
	TRISAbits.TRISA2 = 0;
	TRISBbits.TRISB0=1;
    TRISBbits.TRISB1=1;
    TRISBbits.TRISB2=1;
    TRISBbits.TRISB3=1;
}

void add_char(char ch)
{
    int i = 1;
    for (; i <= 5;i++){
        message[i-1] = message[i]; // shifting na lista pra adicionar o novo elemento
    }
    message[5] = ch;
}

void check_row(int col) 
{
	char chars[3];

    if(
		INPUT_1ROW &&
		INPUT_2ROW &&
		INPUT_3ROW &&
		INPUT_4ROW
	) {
		return;
	}


    if(!INPUT_1ROW) {
		chars[0] = 0b00000110;		//"1"
		chars[1] = 0b01011011;		//"2"
		chars[2] = 0b01001111;		//"3"
	} 
	if(!INPUT_2ROW){
		chars[0] = 0b01100110;		//"4"
		chars[1] = 0b01101101;		//"5"
		chars[2] = 0b01111101;		//"6"
    }
	if(!INPUT_3ROW){
		chars[0] = 0b00000111;		//"7"
		chars[1] = 0b01111111;		//"8"
		chars[2] = 0b01101111;		//"9"
    }
	if(!INPUT_4ROW){
		chars[0] = 0b01000000;      // '*' hífen
		chars[1] = 0b00111111;      // "0"
		chars[2] = 0b00001000;      // '#' underline
    } 

	while(
		!INPUT_1ROW ||
		!INPUT_2ROW ||
		!INPUT_3ROW ||
		!INPUT_4ROW
	){}

	add_char(chars[col]);
}

void scan_input() 
{
    static char i = 0;

    // varrer teclado desativando a coluna que eu quero ler
    if(i == 0) {
        SCAN_KEYBOARD_1COL = 0; 
        SCAN_KEYBOARD_2COL = 1; 
        SCAN_KEYBOARD_3COL = 1; 
    } else if(i == 1) {
        SCAN_KEYBOARD_1COL = 1; 
        SCAN_KEYBOARD_2COL = 0; 
        SCAN_KEYBOARD_3COL = 1;
    } else if(i == 2) {
        SCAN_KEYBOARD_1COL = 1;
        SCAN_KEYBOARD_2COL = 1; 
        SCAN_KEYBOARD_3COL = 0;
    }
    check_row(i);
    if(++i > 2)
        i = 0;
}

void scan_output()
{
    static int i = 0;
    static char aux = 0B00000001;
    if(i % 2)  {
        SCAN_DISPLAY = 0B11111111; // evitar sombra
    }
    else {
        SCAN_DISPLAY = ~aux; // mostra apenas o que está em zero
		CHARACTER = message[i/2];
    	aux = aux << 1;
	}

    if(++i >= 12){
        i = 0;
		aux = 0B00000001;
	}
}

void run()
{
    delay();
    scan_input();
    scan_output();
}

void main() 
{
	configure();
    while(1){
		run();
	}
}