/* lcd header
   By: Mike Gilbert
  
   This is based on the assumption of 4-bit communication with lcd, as opposed to
   8-bit. 4 wires are needed for data (DB4-DB7 on the lcd) and 2 are needed for
   enabling communication and setting write mode (E and RS on the lcd) 
*/

#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define LCD_REGISTER DDRC
#define LCD_PORT PORTC
#define LCD_RS PA6		// RS (read/write pin)
#define LCD_EN PA7		// EN (enable signal pin)
#define LCD_DB4 PC4	// DB4-DB7 (data bus pins for sending data to lcd screen)
#define LCD_DB5 PC5
#define LCD_DB6 PC6
#define LCD_DB7 PC7
#define LCD_SET_RS() (LCD_PORT |= (1<<LCD_RS))		// turn on RS
#define LCD_UNSET_RS() (LCD_PORT &= ~(1<<LCD_RS))	// turn off RS
#define LCD_SET_EN() (LCD_PORT |= (1<<LCD_EN))		// turn on EN
#define LCD_UNSET_EN() (LCD_PORT &= ~(1<<LCD_EN))		// turn off EN

void lcd_init();				// initialize lcd
void lcd_set_bits(uint8_t hex_value);	// simply changes LCD_PORT pins according to hex value
void lcd_cmd(uint8_t hex_value);	// send lcd one of the commands as listed on the datasheet
void lcd_clear();				// clears display
void lcd_print(uint8_t x, uint8_t y, char *str);	// prints str starting at position (x,y)
void lcd_write_char(uint8_t c);
void lcd_set_cursor(uint8_t a, uint8_t b);
#endif