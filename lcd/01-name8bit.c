#include <p18f4520.h>

#define RS LATCbits.LATC5
#define RW LATCbits.LATC6
#define EN LATCbits.LATC7
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

void configure_layout();
void configure_display();
void change_cursor(lineDB row, unsigned char col);
void enable();
void send_instruction(unsigned char command);
void send_data(unsigned char data);
void update_cursor();
void delay_s();
void delay_ms(char ms);
void configure_ports();
void configure();

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

    // DB7 = 0;
    // DB6 = 0;
    // DB5 = 1;
    // DB4 = 1;
    // DB3 = N;
    // DB2 = F;
    // DB1 = X; // whatever
    // DB0 = X; // whatever

    unsigned char command = 0B00110000 | N << 3 | F << 2;
    send_instruction(command);
}

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

    // DB7 = 0;
    // DB6 = 0;
    // DB5 = 0;
    // DB4 = 0;
    // DB3 = 1;
    // DB2 = D;
    // DB1 = C;
    // DB0 = B;

    unsigned char command = 0B00001000 | D << 2 | C << 1 | B;
    send_instruction(command);
}

void change_cursor(lineDB row, unsigned char col)
{
    // 1 linha = 0x80 - 0x8F -> OB10000000 - OB10001111
    // 2 linha = 0xC0 - 0xCF -> OB11000000 - OB11001111

    if (col > 0B1111)
        return;

    send_instruction((unsigned char)row + col);
}

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

void send_instruction(unsigned char command)
{
    RS = INSTRUCTION;
    RW = WRITE;
    DB = command;
    enable();
}

void send_data(unsigned char data)
{

    RS = DATA;
    RW = WRITE;
    DB = data;
    enable();

    // update_cursor();
}

void update_cursor()
{
    static char caracters_writed = 0;
    ++caracters_writed;

    if (caracters_writed == 16)
    {
        change_cursor(LINE_2, 0);
    }
    else if (caracters_writed == 32)
    {
        change_cursor(LINE_1, 0);
        caracters_writed = 0;
    }
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