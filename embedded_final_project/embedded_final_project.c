/*
 * embedded_final_project.c
 *
 * Created: 12/3/2015 4:26:42 PM
 *  Author: mkg
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "lib/uart/uart.h"
#include "lib/print_tools/colors.h"
#include "lib/print_tools/print_tools.h"
#include "lib/spi/spi.h"
#include "lib/sdcard/sdcard.h"
#include "lib/fat32/fat32.h"
#define DELAY		500
#define LED_PORT	PORTA
#define LED1_PIN	_BV(PA0)
#define LED2_PIN	_BV(PA1)


int main(void)
{
	DDRB |= _BV(DDB0);
	
	uart_init();
	
	pt_clear_screen();
	pt_set_color_modifier(FORE(GREEN));
	printf("UART ready.\n");
	
	spi_init();
	printf("SPI ready.\n");
	
	switch(SD_init()) {
		case 0:
			printf("SD ready.\n");
			break;
		case -1: 
			printf("SD Card did not enter idle");
			return;
		case -2:
			printf("SD Card did not exit idle");
			return;
	}
	
	
	
    while(1)
    {
		PORTB ^= _BV(PB0);
		_delay_ms(DELAY);
    }
}