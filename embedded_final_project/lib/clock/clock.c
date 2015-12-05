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

void clock_init() {
	// Prescaler = 64, CTC Mode
	TCCR1B |= (1 << WGM12) | (1 << CS11);
	
	// Initialize Counter
	TCNT1 = 0;
	
	// Initialize Compare Value
	OCR1A = 1000;
	
	// Enable Compare Interrupt
	TIMSK1 |= (1 << OCIE1A);
	
	// Enable timer interrupts
	sei();
}

ISR (TIMER1_COMPA_vect)
{
	// Increment the system clock
	ticks = ticks + 1;
}

// Stop the clock
void clock_stop() {
	TCCR1B = 0;
	TIMSK1 &= ~(1 << OCIE1A);
}

// Get the number of milliseconds in the system clock
unsigned long long clock_get_ms() {
	return ticks;
}