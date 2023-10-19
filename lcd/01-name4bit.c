#include <p18f4520.h>

#define RS LATCbits.LATC5
#define RW LATCbits.LATC6
#define EN LATCbits.LATC7
#define DB7 LATDbits.LATD7
#define DB6 LATDbits.LATD6
#define DB5 LATDbits.LATD5
#define DB4 LATDbits.LATD4
#define DB3 LATDbits.LATD3
#define DB2 LATDbits.LATD2
#define DB1 LATDbits.LATD1
#define DB0 LATDbits.LATD0
#define DB LATD

typedef enum
{
    INSTRUCTION,
    DATA
} typeRS;

typedef enum
{
    WRITE,
    READ
} typeRW;

typedef enum
{
    LINE_1 = 0x80,
    LINE_2 = 0xC0
} lineDB;

void enable();
void configure_layout();
void configure_display();
void send_data(unsigned char data);
void change_cursor(lineDB row, unsigned char col);
void delay_s();
void delay_ms(char ms);
void configure_ports();
void configure();

void enable()
{
    // E-transicao negativa (1->0)
    EN = 1;
    EN = 0;
    delay_ms(1); // min(39us) or wait till RW=READ  say it's okay to send another think

    /*
    RW = READ
    while(busy) {}
    RW = WRITE
    */
}

void configure_layout()
{
    enum
    {
        ONE_LINE_MODE,
        TWO_LINE_MODE
    } N = TWO_LINE_MODE;

    enum
    {
        FONT_5X8,
        FONT_5X11
    } F = FONT_5X8;

    RS = INSTRUCTION;
    RW = WRITE;
    DB7 = 0;
    DB6 = 0;
    DB5 = 1;
    DB4 = 1;
    DB3 = N;
    DB2 = F;
    DB1 = 0; // whatever
    DB0 = 0; // whatever

    enable();
}
// Esperar 39us
void configure_display()
{
    enum
    {
        DISPLAY_OFF,
        DISPLAY_ON
    } D = DISPLAY_ON;

    enum
    {
        CURSOR_OFF,
        CURSOR_ON
    } C = CURSOR_OFF;

    enum
    {
        BLINK_OFF,
        BLINK_ON
    } B = BLINK_OFF;

    RS = INSTRUCTION;
    RW = WRITE;
    DB7 = 0;
    DB6 = 0;
    DB5 = 0;
    DB4 = 0;
    DB3 = 1;
    DB2 = D;
    DB1 = C;
    DB0 = B;

    enable();
}

void send_data(unsigned char data)
{
    RS = DATA;
    RW = WRITE;
    DB = data;
    enable();
}

void change_cursor(lineDB row, unsigned char col)
{
    // 1 linha = 0x80 - 0x8F
    // 2 linha = 0xC0 - 0xCF

    if (col > 0B1111)
        return;

    RS = INSTRUCTION;
    RW = WRITE;
    DB = (unsigned char)row + col;
    enable();

    // (exemplo : 0x80->OB10000000; 0xC0->OB11000000);
}

void delay_s()
{
    unsigned int i;
    for (i = 0; i < 90000; i++)
    {
    }
}

void delay_ms(char ms)
{

    unsigned int i;
    for (; ms > 0; ms--)
    {
        for (i = 0; i < 1500; i++)
        {
        }
    }
}

void configure_ports()
{
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    TRISDbits.TRISD7 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD3 = 0;
    TRISDbits.TRISD2 = 0;
    TRISDbits.TRISD1 = 0;
    TRISDbits.TRISD0 = 0;

    TRISD = 0B00000000;
    ADCON1 = 0B00001111;
}

void configure()
{
    delay_ms(200); // min(30ms)
    configure_layout();
    configure_display();
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
        change_cursor(LINE_1, 0);
    }
}