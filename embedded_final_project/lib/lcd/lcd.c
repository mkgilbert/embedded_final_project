/* lcd commands
   By: Mike Gilbert */

#include "lcd.h"
#include <stdint.h>

/************************************************************************/
/* lcd_cmd()  -- 8-bit communication
   sends the lcd a command as listed in the command table on the datasheet
/************************************************************************/
void _lcd_cmd(uint8_t hex_value){
	LCD_UNSET_RS();				// turn off RS pin
	LCD_PORT = hex_value;		// send data to lcd
	LCD_SET_EN();				// turn on EN pin
	_delay_ms(20);
	LCD_UNSET_EN();				// turn EN pin back off
	_delay_ms(5);
}

/************************************************************************/
/* lcd_init()  -- 8-bit communication                                                  
   The idea here is that you must send several commands to the lcd before it
   is ready to handle displaying data. There are 2 registers being used in this
   implementation (LCD_REGISTER and LCD_WRITE_REGISTER). this is because I am using
   8-bit mode so I need 8 pins for data transfer. There are an extra 2 pins that are
   used to tell the lcd to prepare for receiving a command or data, and actually
   having the lcd store that command or data.
   RS is the pin for telling the lcd which mode to be in (command or data)
   EN (or just E if you want) is the pin for telling the lcd to read all the data pins
   into its local registers (? maybe this isn't 100% quite right)
/************************************************************************/
void lcd_init(){
	// found this code on http://www.8051projects.net/lcd-interfacing/initialization.php
	LCD_REGISTER = 0xFF;								// set whole data register to output
	LCD_WRITE_REGISTER |= (1<<LCD_RS) | (1<<LCD_EN);	// set EN and RS pins as output			
	LCD_PORT = (0x0);									// reset all data pins to 0
	_delay_ms(20);
	
	LCD_PORT = 0x38;	// 2 line, 8-bit, 5x7 dots
	LCD_UNSET_RS();		// tell lcd we are sending a command
	LCD_SET_EN();		// enable H->
	_delay_ms(10);
	LCD_UNSET_EN();
	_delay_ms(10);
	LCD_PORT = 0x0F;
	LCD_UNSET_RS();
	LCD_SET_EN();
	_delay_ms(10);
	LCD_UNSET_EN();
	_delay_ms(10);
	LCD_PORT = 0x01;
	LCD_UNSET_RS();
	LCD_SET_EN();
	_delay_ms(10);
	LCD_UNSET_EN();
	_delay_ms(10);
	LCD_PORT = 0x06;
	LCD_UNSET_RS();
	_delay_ms(10);
	LCD_SET_EN();
	_delay_ms(20);
	
	lcd_clear();
}

/************************************************************************/
/* lcd_clear()	-- 8-bit communication
   just clears all characters off the display
/************************************************************************/
void lcd_clear(){
	_lcd_cmd(0x01);
}

/************************************************************************/
/* lcd_write_char() --8-bit communication                                                     
   For 8-bit communication. Fills the register with data, Sets the RS and E pins high
   and then unsets E.
/************************************************************************/
void lcd_write_char(char c){
	LCD_PORT = c;
	LCD_SET_RS();
	LCD_SET_EN();
	_delay_ms(5);
	LCD_UNSET_EN();
	_delay_ms(1);
}

/************************************************************************/
/* lcd_set_cursor()   -- 8-bit communication                                                     
   places the cursor at column x and row y (where first positions are 0)
/************************************************************************/
void lcd_set_cursor(uint8_t x, uint8_t y){
	x %= LCD_COLS;	// make sure x and y fall into applicable range
	y %= LCD_ROWS;
	uint8_t row = LCD_START_ADDRESS + (0x40*y);	// the row is start plus 0x40 a number of times
												// depending on how many rows there are.
	
	uint8_t address = row + x;					// add the x value to get the column
	_lcd_cmd(address);	
}

/************************************************************************/
/* lcd_print()                                                      
   prints a string beginning at an (x,y) position on the lcd, with either
   wrapping (which clears the screen and continues the messages if need be)
   or just cutting off the text after it fills the screen one time.
   wrap = 1 turns on wrap
   wrap = 0 turns off wrap
   x = 0-indexed column on lcd
   y = 0-indexed row on lcd
/************************************************************************/
void lcd_print(uint8_t wrap, uint8_t x, uint8_t y, char *str){
	//printf("printing to lcd: (%"PRIu8", %"PRIu8")\n", x, y);
	lcd_set_cursor(x, y);
	
	x %= LCD_COLS;					// make sure x and y are in bounds
	y %= LCD_ROWS;
	uint8_t lines_printed = 0;		// keep track of how many lines so we can clear screen after 2
	char i = 0;						// string iterator
	
	while (*(str+i) != '\0') {
		if (x == LCD_COLS){			// if at the end of the line
			x = 0;					// reset x to beginning
			lines_printed++;
			if (lines_printed > 0 && y == LCD_ROWS-1) { // wait and clear screen if on last row
				//printf(" [delay 3s] [clear] x->%"PRIu8 " y->%"PRIu8"\n", x, y);
				
				if (wrap) {			// if we are doing 'wrap' printing
					_delay_ms(LCD_TEXT_WRAP_DELAY);
					lcd_clear();
				}
				else				// not doing wrap printing, so just exit due to hitting end of line
					break;
			}
			y = (y+1) % LCD_ROWS;	// move y to next row
			//printf("\n[move (%"PRIu8", %"PRIu8")]\n", x, y);
			lcd_set_cursor(x, y);
		}
		lcd_write_char(*(str+i));
		//printf("%c", *(str+i));
		i++;
		x++;
	}
	//printf("\n");
}

void lcd_print_ctr(uint8_t row, char *str) {
	
	row %= LCD_ROWS; // make sure row in range
	
	// get total length of string first;
	char i = 0;
	while (*(str+i) != '\0') { i++; }
	char len = i;
	//printf("len: %d\n", len);
	
	// get center position
	// string same size as the line
	if (len == LCD_COLS){
		lcd_print(0, 0, row, str);
	} 
	// string smaller than line
	else if (len < LCD_COLS) {
		char left_offset = (LCD_COLS - len)/2;
		lcd_print(0, left_offset, row, str);
	}
	// string too long for line
	else {
		lcd_print(0, 0, row, str); // just print with no wrap until out of room
	}
}
