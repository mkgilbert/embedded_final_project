/*
 * clock.c
 *
 * Created: 11/4/2015 9:31:32 PM
 *  Author: Harrison
 */ 

#include "clock.h"
#include <avr/interrupt.h>
#include <avr/io.h>

unsigned long long volatile ticks;
uint8_t enabled = 0;

void clock_init() {
	// CTC Mode
	TCCR0A = (1 << WGM01);
	
	// Prescaler
	TCCR0B = (1 << CS02);
	
	// Initialize Counter
	TCNT0 = 0;
	
	// Initialize Compare Value
	OCR0A = 111;
	
	// Enable Compare Interrupt
	TIMSK0 = (1 << OCIE0A);
	
	// Enable timer interrupts
	sei();
	
	enabled = 1;
}

ISR (TIMER0_COMPA_vect)
{
	// Increment the system clock
	ticks = ticks + 1;
}

// Stop the clock
void clock_stop() {
	TCCR1B = 0;
	TIMSK1 &= ~(1 << OCIE1A);
	
	enabled = 0;
}

// Get the number of milliseconds in the system clock
unsigned long long clock_get_ms() {
	return ticks;
}

uint8_t clock_is_enabled() {
	return enabled;
}