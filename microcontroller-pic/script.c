#include <p18f4520.h>

// RS=0 instrucao
// RS=1 dado

// RW=0 escrita
// RW=1 leitura

// E-transicao negativa (1->0)
// VSS=0
// VDD=1

// 1 linha = 0x80 - 0x8F
// 2 linha = 0xC0 - 0xCF

// Fluxograma:
// Esperar 30 ms
// Funcao de configurar linhas e fonte {
//      RS=0
//      RW=0
//      DB7=0
//      DB6=0
//      DB5=1
//      DB4=1
//      DB3=1 (2 line-mode)
//      DB2=0 (5x8 font)
//      DB1=x
//      DB0=x
// }
// Esperar 39us
// Funcao de configurar display {
//      RS=0
//      RW=0
//      DB7=0
//      DB6=0
//      DB5=0
//      DB4=0
//      DB3=1
//      DB2=1 (display on)
//      DB1=1 (curson on)
//      DB0=0 (blink off)
// }
// Enviar dado{
//      RS=1
//      RW=0
//      DB7-DB0=dado em ASCII
//}
// Mudar cursor{
//      RS=0
//      RW=0
//      DB7=1
//      DB6-DB0=END (exemplo: 0x80 -> OB10000000 ; 0xC0 -> OB11000000)
//}
0100 / 0100 61 - 0110 / 0001 76 - 0111 / 0110 69 - 0110 / 1001

                                                       void
                                                       delay_s(void)
{
    unsigned int i;
    for (i = 0; i < 90000; i++)
    {
    }
}

void configure()
{
    // delay_ms(200); // espera alimentação do lcd estabilizar (min 30 ms)
}

void main()
{
    configure();
    while (1)
    {
    }
}