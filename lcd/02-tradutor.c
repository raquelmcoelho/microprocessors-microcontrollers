#include <p18f4520.h>

#define RS1 LATCbits.LATC5
#define RW1 LATCbits.LATC6
#define EN1 LATCbits.LATC7
#define DB1 LATD

#define RS2 LATAbits.LATA2
#define RW2 LATAbits.LATA1
#define EN2 LATAbits.LATA0
#define DB2 LATB

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

typedef enum
{
    LCD1,
    LCD2
} LCD;

const enum {
    MODE_4_BIT,
    MODE_8_BIT
} DL = MODE_4_BIT;

const enum {
    ONE_LINE_MODE,
    TWO_LINE_MODE
} N = TWO_LINE_MODE;

const enum {
    FONT_5X8,
    FONT_5X11
} F = FONT_5X8;

const enum {
    DISPLAY_OFF,
    DISPLAY_ON
} D = DISPLAY_ON;

enum
{
    CURSOR_OFF,
    CURSOR_ON
} C = CURSOR_OFF;

const enum {
    BLINK_OFF,
    BLINK_ON
} B = BLINK_OFF;

LCD lcd;

void configure_layout();
void configure_display();
void clear_display();
void change_cursor(unsigned char row, unsigned char col);
void enable();
void send(typeRS type, unsigned char data);
void delay_ms(unsigned long long int ms);
void configure_ports();
void configure();

void configure_layout()
{
    // DB7 | DB6 | DB5 | DB4 | DB3 | DB2 | DB1 | DB0
    //  0  |  0  |  1  | DL  |  N  |  F  |  X  |  X

    unsigned char command = 0B00100000 | DL << 4 | N << 3 | F << 2;
    send(INSTRUCTION, command);
}

void configure_display()
{
    // DB7 | DB6 | DB5 | DB4 | DB3 | DB2 | DB1 | DB0
    //  0  |  0  |  0  |  0  |  1  |  D  |  C  |  B

    unsigned char command = 0B00001000 | D << 2 | C << 1 | B;
    send(INSTRUCTION, command);
}

void clear_display()
{
    send(INSTRUCTION, 0B00000001);
}

void change_cursor(unsigned char row, unsigned char col)
{
    // 1 linha = 0x80 - 0x8F -> OB10000000 - OB10001111
    // 2 linha = 0xC0 - 0xCF -> OB11000000 - OB11001111

    if (col > 0B1111)
        return;

    send(INSTRUCTION, row + col);
}

void enable()
{
    // E-transicao negativa (1->0)
    if (lcd == LCD1)
    {
        EN1 = 1;
        EN1 = 0;
    }
    else if (lcd == LCD2)
    {
        EN2 = 1;
        EN2 = 0;
    }
    delay_ms(1);
}

void send(typeRS type, unsigned char command)
{
    if (lcd == LCD1)
    {
        RS1 = type;
        RW1 = WRITE;
        DB1 = command;
        enable();

        if (DL == MODE_4_BIT)
        {
            DB1 = command << 4;
            enable();
        }
    }
    else if (lcd == LCD2)
    {
        RS2 = type;
        RW2 = WRITE;
        DB2 = command;
        enable();

        if (DL == MODE_4_BIT)
        {
            DB2 = command << 4;
            enable();
        }
    }
}

void delay_ms(unsigned long long int ms)
{

    unsigned char i;
    for (; ms > 0; ms--)
    {
        for (i = 0; i < 145; i++)
        {
        }
    }
}

void configure_ports()
{
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    TRISAbits.TRISA2 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA0 = 0;

    TRISD = 0B00000000;
    TRISB = 0B00000000;
    ADCON1 = 0B00001111;
}

void configure()
{
    delay_ms(200); // min(30ms) + min(40ms)

    if (DL == MODE_4_BIT)
        send(INSTRUCTION, 0B00000010);
    configure_layout();
    configure_display();
    if (DL == MODE_4_BIT)
        send(INSTRUCTION, 0B00000100 | C << 1);
    clear_display();
}

void main()
{

    configure_ports();
    lcd = LCD1;
    configure();
    lcd = LCD2;
    configure();

    while (1)
    {
        lcd = LCD1;
        send(DATA, 'O');
        send(DATA, 'L');
        send(DATA, 'A');
        change_cursor(LINE_1, 0);

        lcd = LCD2;
        send(DATA, 'H');
        send(DATA, 'E');
        send(DATA, 'L');
        send(DATA, 'L');
        send(DATA, 'O');
        change_cursor(LINE_1, 0);
    }
}