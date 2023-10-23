#include <p18f4520.h>

#define RS LATCbits.LATC5
#define RW LATCbits.LATC6
#define EN LATCbits.LATC7
#define porta_lcd LATD

int caracters_writed = 0;

void enable();
void send_instruction(unsigned char command);
void send_data(unsigned char data);
void delay_ms(char ms);
void configure_ports();
void configure();

void send_instruction(unsigned char command)
{
    RS = 0;
    RW = 0;
    EN = 1;
    porta_lcd = command;
    EN = 0;
    delay_ms(1);
}

void send_data(unsigned char data)
{
    RS = 1;
    RW = 0;
    EN = 1;
    porta_lcd = data;
    EN = 0;
    delay_ms(1);

    ++caracters_writed;

    if (caracters_writed == 16)
    {
        send_instruction(0B11000000); // vai para a segunda linha
    }
    else if (caracters_writed == 32)
    {
        send_instruction(0B10000000); // vai para a primeira linha
        caracters_writed = 0;
    }
}

void delay_ms(char ms)
{

    unsigned int i;
    for (; ms > 0; ms--)
    {
        for (i = 0; i < 135; i++)
        {
        }
    }
}

void configure_ports()
{
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    TRISD = 0B00000000;
    ADCON1 = 0B00001111;
}

void configure()
{
    delay_ms(200);                // min(30ms)
    send_instruction(0B00111000); // 2 linhas 5x8
    send_instruction(0B00001110); // cursor
    send_instruction(0B00000001); // limpa lcd
}

void main()
{
    configure_ports();
    configure();
    while (1)
    {
        send_data('R');
        send_data('A');
        send_data('Q');
        send_data('U');
        send_data('E');
        send_data('L');
        send_instruction(0B10000000); // volta cursor pro começo
    }
}