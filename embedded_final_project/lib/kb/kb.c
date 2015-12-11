/*
 * kb.c
 *
 * Created: 12/10/2015 12:47:56 AM
 *  Author: Harrison
 */ 

#include "kb.h"
#include "scancodes.h"
#include "lib/port_helpers/port_helpers.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

uint8_t volatile edge, bitcount;
uint8_t volatile data, is_up = 0, shift = 0, mode = 0;

unsigned char volatile buffer[KB_BUFFER_SIZE];
unsigned char* volatile buffer_in;
unsigned char* volatile buffer_out;
uint8_t volatile buffer_cnt;

void kb_init() {
	
	PCICR |= (1 << KB_PCIE);
	KB_PCMSK = (1 << KB_INTERRUPT);
	
	edge = 0;
	bitcount = 11;
	
	buffer_in = buffer;
	buffer_out = buffer;
	buffer_cnt = 0;
	
	sei();
	
}

void kb_putchar(unsigned char c) {
	if (buffer_cnt < KB_BUFFER_SIZE) {
		*(buffer_in++) = c;
		buffer_cnt++;
		
		// Wrap pointer
		if (buffer_in >= buffer + KB_BUFFER_SIZE)
			buffer_in = buffer;
	}
}

uint8_t kb_haschar() {
	return buffer_cnt > 0;
}

unsigned char kb_getchar() {
	unsigned char byte;
	
	while (!kb_haschar());
	
	byte = *(buffer_out++);
	
	if (buffer_out >= buffer + KB_BUFFER_SIZE)
		buffer_out = buffer;
	
	buffer_cnt--;
	
	return byte;
}

void kb_decode(unsigned char data) {
	uint8_t i;
	
	if (!is_up) {
		switch (data) {
			case 0xF0:
				// Key-up identifier
				is_up = 1;
				break;
			
			case 0x12:
				// Left shift
				shift = 1;
				break;
			
			case 0x59:
				// Right shift
				shift = 1;
				break;
			
			default:
				if (mode == 0 || mode == 3) {
					if (!shift) {
						// Shift not pressed, table look-up
						for (i = 0; pgm_read_byte(&(unshifted[i][0])) != data && pgm_read_byte(&(unshifted[i][0])); i++);
						if (pgm_read_byte(&(unshifted[i][0])) == data) {
							kb_putchar(pgm_read_byte(&(unshifted[i][1])));
						}
					} else {
						// Shift pressed
						for (i = 0; pgm_read_byte(&(shifted[i][0])) != data && pgm_read_byte(&(shifted[i][0])); i++);
						if (pgm_read_byte(&(shifted[i][0])) == data) {
							kb_putchar(pgm_read_byte(&(shifted[i][1])));
						}
					}
				} else {
					
				}
				break;
		}
	} else {
		is_up = 0;
		switch (data) {
			case 0x12:
				// Left shift
				shift = 0;
				break;
			
			case 0x59:
				// Right shift
				shift = 0;
				break;
		}
	}
}

ISR(KB_PCINT_VECT) {
	if (!edge) {
		// Falling edge - read data
		edge = 1;
		if (bitcount < 11 && bitcount > 2) {
			data = (data >> 1);
			if (PORT_READ(KB_DATA))
				data = data | 0x80;
		}
	}
	else {
		// Rising edge
		edge = 0;
		if (--bitcount == 0) {
			kb_decode(data);
			bitcount = 11;
		}
	}
}