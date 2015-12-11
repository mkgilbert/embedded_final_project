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

uint8_t snes_is_pressed(uint8_t controller, uint8_t button){
	if (controller == SNES_CONTROLLER1){
		return data_c1 & (1<<button) > 0;
	}
	else if (controller == SNES_CONTROLLER2){
		return data_c2 & (1<<button) > 0;
	}
	return 0;
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
	
	snes_update();
}

void snes_update(){
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
		_delay_us(6);
		
		data_c1 |= (PORT_READ (SNES_DATA1) << i);
		data_c2 |= (PORT_READ (SNES_DATA2) << i);
		
		PORT_SET (SNES_CLOCK);
		_delay_us(6);
	}
	
	data_c1 = ~data_c1;
	data_c2 = ~data_c2;
}