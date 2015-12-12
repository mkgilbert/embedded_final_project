/* lcd header
   By: Mike Gilbert
  
   This is based on the assumption of 4-bit communication with lcd, as opposed to
   8-bit. 4 wires are needed for data (DB4-DB7 on the lcd) and 2 are needed for
   enabling communication and setting write mode (E and RS on the lcd) 
*/

#ifndef LCD_H
#define LCD_H
#include "system.h"
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define LCD_REGISTER DDRC		// all 8 pins of port needed for 8-bit communication
#define LCD_WRITE_REGISTER DDRA	// there are 2 extra pins required for writing/enabling
#define LCD_PORT PORTC
#define LCD_WRITE_PORT PORTA
#define LCD_RS PA7		// RS (read/write pin)
#define LCD_EN PA6		// EN (enable signal pin)
#define LCD_ROWS 2		// 16 x 2 display
#define LCD_COLS 16
#define LCD_START_ADDRESS 0x80	// address in lcd of the upper left cell
								// every subsequent row should start +0x40 after the previous
#define LCD_TEXT_WRAP_DELAY 3000 // delay in ms for lcd to wait before clearing screen for new lines

/************************************************************************/
/* Macros */                                                         
/************************************************************************/
#define LCD_SET_RS() (LCD_WRITE_PORT |= (1<<LCD_RS))		// turn on RS
#define LCD_UNSET_RS() (LCD_WRITE_PORT &= ~(1<<LCD_RS))	// turn off RS
#define LCD_SET_EN() (LCD_WRITE_PORT |= (1<<LCD_EN))		// turn on EN
#define LCD_UNSET_EN() (LCD_WRITE_PORT &= ~(1<<LCD_EN))		// turn off EN

#define LCD_CURSOR_BLINK_ON() (_lcd_cmd(0x0F))
#define LCD_CURSOR_BLINK_OFF() (_lcd_cmd(0x0E))
#define LCD_CURSOR_ON() (_lcd_cmd(0x0E))
#define LCD_CURSOR_OFF() (_lcd_cmd(0x0C))
			
/************************************************************************/
/* function prototypes  */
/************************************************************************/					
void lcd_init();								// initialize lcd
void _lcd4_set_bits(uint8_t hex_value);			// simply changes LCD_PORT pins according to hex value
void _lcd_cmd(uint8_t hex_value);				// send lcd one of the commands as listed on the datasheet
void lcd_clear();								// clears display
void lcd_print(uint8_t wrap, uint8_t x, uint8_t y, char *str);// prints str starting at position (x,y)
												// wrap = 1 or 0 and chooses between clearing screen and wrapping
												// text to next screen
void lcd_print_ctr(uint8_t row, char *str);		// centers text on a given row (starting at 0)
void lcd_write_char(char c);
void lcd_set_cursor(uint8_t x, uint8_t y);		// moves cursor to position (x,y) on screen

#endif