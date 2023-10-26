#include <p18f4520.h>

#define RS LATCbits.LATC5
#define RW LATCbits.LATC6
#define EN LATCbits.LATC7
#define LED LATCbits.LATC4
#define DB LATD

#define SCAN_KEYBOARD_1COL LATAbits.LATA0
#define SCAN_KEYBOARD_2COL LATAbits.LATA1
#define SCAN_KEYBOARD_3COL LATAbits.LATA2
#define INPUT_1ROW PORTBbits.RB0
#define INPUT_2ROW PORTBbits.RB1
#define INPUT_3ROW PORTBbits.RB2
#define INPUT_4ROW PORTBbits.RB3

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

const char pass[9] = "123#*456";
char input[9];

void configure_layout();
void configure_display();
void clear_display();
void change_cursor(unsigned char row, unsigned char col);
void enable();
void send(typeRS type, unsigned char data);
void delay_ms(unsigned long long int ms);
void configure_ports();
void configure();
void add_char(char ch);
void check_row(int col);
void scan_input();
void scan_output();
void check_pass();
void show_message(char success);

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
    EN = 1;
    EN = 0;
    delay_ms(1); // min(39us) or wait till RW=READ  say it's okay to send another think

    /*
    RW = READ
    while(busy) {}
    RW = WRITE
    */
}
void send(typeRS type, unsigned char command)
{
    RS = type;
    RW = WRITE;
    DB = command;
    enable();

    if (DL == MODE_4_BIT)
    {
        DB = command << 4;
        enable();
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
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    TRISAbits.TRISA2 = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    TRISD = 0B00000000;
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

void check_pass()
{
    char i = 0;
    char success = 1;
    for (i = 0; i < 8; i++)
    {
        if (pass[i] != input[i])
            success = 0;

        input[i] = 0;
    }
    show_message(success);
}

void show_message(char success)
{
    clear_display();
    change_cursor(LINE_1, 0);
    if (success)
    {
        send(DATA, 'A');
        send(DATA, 'C');
        send(DATA, 'E');
        send(DATA, 'I');
        send(DATA, 'T');
        send(DATA, 'O');
        LED = 1;
    }
    else
    {
        send(DATA, 'N');
        send(DATA, 'E');
        send(DATA, 'G');
        send(DATA, 'A');
        send(DATA, 'D');
        send(DATA, 'O');
        LED = 0;
    }
    delay_ms(5000);
}

void add_char(char ch)
{
    static char i = 0;
    input[i] = ch;
    i++;

    if (i == 8)
    {
        i = 0;
        check_pass();
    }
}

void check_row(int col)
{
    char chars[3];

    if (
        INPUT_1ROW &&
        INPUT_2ROW &&
        INPUT_3ROW &&
        INPUT_4ROW)
    {
        return;
    }

    if (!INPUT_1ROW)
    {
        chars[0] = '1';
        chars[1] = '2';
        chars[2] = '3';
    }
    if (!INPUT_2ROW)
    {
        chars[0] = '4';
        chars[1] = '5';
        chars[2] = '6';
    }
    if (!INPUT_3ROW)
    {
        chars[0] = '7';
        chars[1] = '8';
        chars[2] = '9';
    }
    if (!INPUT_4ROW)
    {
        chars[0] = '*';
        chars[1] = '0';
        chars[2] = '#';
    }

    while (
        !INPUT_1ROW ||
        !INPUT_2ROW ||
        !INPUT_3ROW ||
        !INPUT_4ROW)
    {
    }

    add_char(chars[col]);
}

void scan_input()
{
    static char i = 0;

    // varrer teclado desativando a coluna que eu quero ler
    if (i == 0)
    {
        SCAN_KEYBOARD_1COL = 0;
        SCAN_KEYBOARD_2COL = 1;
        SCAN_KEYBOARD_3COL = 1;
    }
    else if (i == 1)
    {
        SCAN_KEYBOARD_1COL = 1;
        SCAN_KEYBOARD_2COL = 0;
        SCAN_KEYBOARD_3COL = 1;
    }
    else if (i == 2)
    {
        SCAN_KEYBOARD_1COL = 1;
        SCAN_KEYBOARD_2COL = 1;
        SCAN_KEYBOARD_3COL = 0;
    }
    check_row(i);
    if (++i > 2)
        i = 0;
}

void scan_output()
{
    char i = 0;
    clear_display();
    change_cursor(LINE_1, 0);
    for (i = 0; i < 8; i++)
    {
        send(DATA, input[i]);
    }
}

void main()
{
    configure_ports();
    configure();
    while (1)
    {
        delay_ms(1);
        scan_input();
        scan_output();
    }
}