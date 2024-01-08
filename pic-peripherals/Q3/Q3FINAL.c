/* Marcelo De Araújo */

#include <p18f4520.h>

unsigned char periodo = 20;
unsigned char inc_dec = 1;
unsigned int alarme = 0;

void alta_prioridade (void);
void transmite_uart (int);

void delay_ms(int ms) {

	unsigned int i;

	for (;ms > 0; ms--) {
		for (i = 0; i < 128; i++) {}		
        //tempo de loop = 1ms
		i++;i++;i++;
	}
}

void configura_pic(void) {
	
	TRISB = 0b11111111;
	TRISCbits.RC2 = 0;
	ADCON1 = 0x0F;

	// configurando o PWM 1
	
	//SAIDA PWM
	TRISCbits.RC2 = 0;			
    	//pino do PWM 1
			
	//TIMER2
	
	T2CONbits.T2CKPS1 = 1;
	
	//prescaler do TMR2 de 1:16  (8us a 8MHz)
	T2CONbits.T2CKPS0 = 1;
	T2CONbits.TMR2ON = 1;	
	//liga TMR2

	// INTERRUPT
	INTCONbits.GIE = 1; 
	// Habilita sistema de interrupções

	INTCONbits.RBIE = 1; 
	// Habilita INT0
	INTCONbits.RBIF = 0;
	 // Espera Interupt
	INTCON2bits.RBIP = 1; 
	// Alta prioridade

	INTCON2bits.INTEDG0 = 1;
	 // Habilita na subida do pino

	// Configura UART
	TRISCbits.RC6 = 0;
	TRISCbits.RC7 = 1;
	
	TXSTA = 0b00100100;
	RCSTA = 0b10010000;
	BAUDCON = 0b00000000;
	SPBRG = 50;
		
}

void transmite_uart(int zona) {

	int i;
	char dado[17] = 

{
	'Z',
	'o',
	'n',
	'a',
	' ',
	zona+48,
	' ',
	'(',
	'0',
	'1',
	':',
	'1',
	'1',
	':',
	'1',
	'1',
	')'
	};

	for(i = 0; i < 17; i++) {
		TXREG = dado[i];
		while(TXSTAbits.TRMT);
		delay_ms(1);
	}
}

#pragma code vetor_alta = 0x0008

void interrupt_alta (void) {
	_asm GOTO alta_prioridade _endasm
}

#pragma code

#pragma interrupt alta_prioridade

void alta_prioridade (void) {

	if(INTCONbits.RBIF) {
		if(PORTBbits.RB4) {
			alarme =~ alarme;
			transmite_uart(1);
			INTCONbits.RBIF = 0;
		}

		if(PORTBbits.RB5) {
			alarme =~ alarme;
			transmite_uart(2);
			INTCONbits.RBIF = 0;
		}

		if(PORTBbits.RB6) {
			alarme =~ alarme;
			transmite_uart(3);
			INTCONbits.RBIF = 0;
		}

		if(PORTBbits.RB7) {
			alarme =~ alarme;
			transmite_uart(4);
			INTCONbits.RBIF = 0;
		}
		INTCONbits.RBIF = 0;
	}
}

void main() {

	configura_pic();

	while(1) {	

		PR2 = 0; //MODULO PWM			
		CCPR1L = periodo/2; //Periodo do pwm  8us x PR2 = 1Khz
		
		CCP1CONbits.CCP1M3 = 0;	//Duty cicle do PWM (min)

		//modo PWM 1
		CCP1CONbits.CCP1M2 = 0;			
		CCP1CONbits.CCP1M1 = 0;			
		CCP1CONbits.CCP1M0 = 0;	

		while(alarme) {

			PR2 = periodo; //MODULO PWM	
			CCPR1L = periodo/2; //Periodo do pwm  8us x PR2 = 1Khz
		
			CCP1CONbits.CCP1M3 = 1;	//Duty cicle do PWM (min)

			//modo PWM 1
			CCP1CONbits.CCP1M2 = 1;			
			CCP1CONbits.CCP1M1 = 0;			
			CCP1CONbits.CCP1M0 = 0;	
	
			PR2 = periodo; //periodo do PWM		

			CCPR1L = periodo/10;
		
			delay_ms(20); //duty cycle do PWM
			
			if (inc_dec == 1) {
				periodo = periodo+1;
			}

			else {
				periodo = periodo-1;
			}

			if (periodo>240) {
				inc_dec = 0;
			}

			if (periodo<30) {
				inc_dec = 1;
			}
		}
	}
}
