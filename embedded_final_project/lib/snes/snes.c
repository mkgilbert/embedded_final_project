/*
 * snes.c
 *
 * Created: 12/10/2015 3:36:15 PM
 *  Author: Matt Q
 */ 


#include "system.h"
#include <avr/io.h>
#include <stdio.h>
#include "lib/port_helpers/port_helpers.h"
#include "snes.h"
#include <avr/delay.h>

uint16_t data_c1;
uint16_t data_c2;

uint16_t data_c1_prev;
uint16_t data_c2_prev;

uint8_t snes_is_pressed(uint8_t controller, uint8_t button){
	
	uint16_t diff = 0x0000;
	
	if (controller == SNES_CONTROLLER1){
		diff = (data_c1 ^ data_c1_prev) & data_c1;
	}
	else if (controller == SNES_CONTROLLER2){
		diff = (data_c2 ^ data_c2_prev) & data_c2;
	}
	
	return (diff & (1 << button)) > 0;
}

void snes_init(){
	//Set pins as inputs
	PORT_INPUT (SNES_DATA1);
	PORT_INPUT (SNES_DATA2);
	//Set pins as output
	PORT_OUTPUT (SNES_LATCH);
	PORT_OUTPUT (SNES_CLOCK);
	
	//Enables pull up resistors for each input
	PORT_SET (SNES_DATA1);
	PORT_SET (SNES_DATA2);
	
	data_c1_prev = data_c2_prev = data_c1 = data_c2 = 0x0000;
	
	snes_update();
}

void snes_update(){
	
	data_c1_prev = data_c1;
	data_c2_prev = data_c2;
	
	data_c1 = 0x0000;
	data_c2 = 0x0000;

	//pull latch to read new data
	PORT_SET (SNES_LATCH);
	_delay_us(12);
	PORT_UNSET (SNES_LATCH);
	_delay_us(6);

	//read bits
	for(int i = 0; i < 16; i++){
		PORT_UNSET (SNES_CLOCK);
		//_delay_us(6);
		
		data_c1 |= (PORT_READ (SNES_DATA1) << i);
		data_c2 |= (PORT_READ (SNES_DATA2) << i);
		
		PORT_SET (SNES_CLOCK);
		//_delay_us(6);
	}
	
	data_c1 = ~data_c1;
	data_c2 = ~data_c2;
}