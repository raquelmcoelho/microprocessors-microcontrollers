#include <p18f4520.h>

#define HOURS LATC
#define MINUTES LATD
#define SECONDS LATB
#define ALARM_MODE PORTAbits.RA0
#define HOURS_ADJUST PORTAbits.RA2
#define MIN_ADJUST PORTAbits.RA4
#define BUZZER LATAbits.LATA5

char _actual_seconds = 0;
char _actual_minutes = 0;
char _actual_hours = 0;

char _alarm_seconds = 59;
char _alarm_minutes = 59;
char _alarm_hours = 23;

char *_seconds = &_actual_seconds;
char *_minutes = &_actual_minutes;
char *_hours = &_actual_hours;

void delay_s(int);
void show_bcd();
void increment_hours();
void increment_minutes();
void increment_seconds();
void increment_clock();
void check_alarm();
void check_mode();
void check_adjust();
void run();
void configure();

void delay_s(int s)
{
	unsigned long int i;
	for (; s > 0; s--)
	{
		for (i = 0; i < 41650; i++)
		{
		};
	}
}

void show_bcd()
{
	/*

		_hours = 25
		_hours/10 = 2 (dezena)
		_hours%10 = 5 (unidade)

		dezena=2 (00000010), unidade=5 (00000101)

		dezena  = 00100000  << 4
		unidade = 00000101
		   OU   = 00100101
	*/
	HOURS = ((((*_hours) / 10) << 4) | ((*_hours) % 10));
	MINUTES = ((((*_minutes) / 10) << 4) | ((*_minutes) % 10));
	SECONDS = ((((*_seconds) / 10) << 4) | ((*_seconds) % 10));
}

void increment_hours()
{
	if (++(*_hours) == 24)
	{
		(*_hours) = (*_minutes) = (*_seconds) = 0;
	}
}

void increment_minutes()
{
	if (++(*_minutes) == 60)
	{
		(*_minutes) = (*_seconds) = 0;
		increment_hours();
	}
}

void increment_seconds()
{
	if (++(*_seconds) == 60)
	{
		(*_seconds) = 0;
		increment_minutes();
	}
}

void increment_clock()
{
	increment_seconds();
}

void check_alarm()
{
	if (
		_actual_hours == _alarm_hours &&
		_actual_minutes == _alarm_minutes &&
		_actual_seconds % 2 // para tocar de 1 em 1 segundo o alarme
	)
	{
		BUZZER = 1;
	}
}

void check_mode()
{
	static int seconds_alarm_button_activated = 0;

	if (ALARM_MODE)
		seconds_alarm_button_activated++;
	else
		seconds_alarm_button_activated = 0;

	if (seconds_alarm_button_activated >= 2)
	{
		_seconds = &_alarm_seconds;
		_minutes = &_alarm_minutes;
		_hours = &_alarm_hours;
	}
	else
	{
		_seconds = &_actual_seconds;
		_minutes = &_actual_minutes;
		_hours = &_actual_hours;

		check_alarm();
		increment_clock();
	}
}

void check_adjust()
{
	if (HOURS_ADJUST)
		increment_hours();

	if (MIN_ADJUST)
		increment_minutes();
}

void run()
{
	BUZZER = 0;
	check_adjust();
	check_mode();
	show_bcd();
	delay_s(1);
}

void configure()
{
	//  76543210
	TRISC = 0B00000000;	  //	hours
	TRISD = 0B00000000;	  //	minutes
	TRISB = 0B00000000;	  //	seconds
	TRISAbits.TRISA0 = 1; //	mode (input)
	TRISAbits.TRISA2 = 1; //	hour adjust (input)
	TRISAbits.TRISA4 = 1; //	minutes adjust (input)
	TRISAbits.TRISA5 = 0; //	buzzer (output)
	ADCON1 = 0B00001111;  //	DESABILITA ANALOGICOS
}

void main()
{
	configure();
	while (1)
	{
		run();
	}
}