/* lcd header
   By: Mike Gilbert
  
   This is based on the assumption of 4-bit communication with lcd, as opposed to
   8-bit. 4 wires are needed for data (DB4-DB7 on the lcd) and 2 are needed for
   enabling communication and setting write mode (E and RS on the lcd) 
*/

#ifndef LCD_H
#define LCD_H

#define LCD_PORT PORTC		// lcd will connect to register C
#define LCD_RS _BV(PC0)		// RS (read/write pin)
#define LCD_EN _BV(PC1)		// EN (enable signal pin)
#define LCD_DB4 _BV(PC4)	// DB4-DB7 (data bus pins for sending data to lcd screen)
#define LCD_DB5 _BV(PC5)
#define LCD_DB6 _BV(PC6)
#define LCD_DB7 _BV(PC7)

void lcd_init();				// initialize lcd
void lcd_set_bits(uint8_t hex_value);	// simply changes LCD_PORT pins according to hex value
void lcd_cmd(uint8_t hex_value);	// send lcd one of the commands as listed on the datasheet
void lcd_clear();				// clears display
void lcd_print(uint8_t x, uint8_t y, char *str);	// prints str starting at position (x,y)


#endif