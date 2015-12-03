/*
 * embedded_final_project.c
 *
 * Created: 12/3/2015 4:26:42 PM
 *  Author: mkg
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#define DELAY		500
#define LED_PORT	PORTA
#define LED1_PIN	_BV(PA0)
#define LED2_PIN	_BV(PA1)


int main(void)
{
	DDRB |= _BV(DDB0);
	
    while(1)
    {
		PORTB ^= _BV(PB0);
		_delay_ms(DELAY);
    }
}