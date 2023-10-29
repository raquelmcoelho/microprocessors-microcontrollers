#include <p18f4520.h>
	
#define sinal			PORTAbits.RA6
int periodo, frequencia;
void delay();
void mede_periodo(void);
void calcula_frequencia(void);
void mostra_frequencia(void);

void delay() {
	unsigned int i;
	for(i=0;i<4;i++){}
}

void mede_periodo(void) {
	periodo=0;

	while(sinal) {};
	while(!sinal) {};	//detecta a borda de subida do sinal (0 -> 1)

	while(sinal) {
		delay();
		periodo++;
	}

	while(!sinal) {
		delay();
		periodo++;
	}
}

void calcula_frequencia(void){
	frequencia = 10000 / periodo;
}


void mostra_frequencia(void){
	if(frequencia > 99){
		frequencia = 99;
	}else if(frequencia <= 10){
		frequencia = 10;
	}
	
	LATC = ((frequencia/10) << 4 | (frequencia%10));
}


void frequencimetro(void){
	mede_periodo ();
	calcula_frequencia ();
	mostra_frequencia ();
}

void main () {
	ADCON1=0B00001111;	//DESABILITA ANALOGICOS
	TRISC=0B00000000;	//display como saida
	TRISAbits.RA6=1;	//sinal de frequencia configurado como entrada

	while (1) {
        delay();
		frequencimetro();
	}
}
