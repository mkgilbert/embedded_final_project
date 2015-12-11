/* lcd commands
   By: Mike Gilbert */

#include "lcd.h"

/************************************************************************/
/* lcd_set_bits()                                                     
   This sets bits corresponding to whatever command you want to send to the
   lcd. The sending of a command has other procedures, so the setting of the bits
   was put in a separate function.
   Only set the 4 bits we are using for communication. Don't touch the rest on
   the register (0-3)
/************************************************************************/
void lcd_set_bits(uint8_t hex_value){
	if (hex_value & 1)
		LCD_PORT |= (1<<LCD_DB4);
	else
		LCD_PORT &= ~(1<<LCD_DB4);
	if (hex_value & 2)
		LCD_PORT |= (1<<LCD_DB5);
	else
		LCD_PORT &= ~(1<<LCD_DB5);
	if (hex_value & 4)
		LCD_PORT |= (1<<LCD_DB6);
	else
		LCD_PORT &= ~(1<<LCD_DB6);
	if (hex_value & 8)
		LCD_PORT |= (1<<LCD_DB7);
	else
		LCD_PORT &= ~(1<<LCD_DB7);
}

/************************************************************************/
/* lcd_cmd()
   sends the lcd a command as listed in the command table on the datasheet
/************************************************************************/
void lcd_cmd(uint8_t hex_value){
	LCD_PORT &= ~(1<<LCD_RS);	// turn off RS pin
	lcd_set_bits(hex_value);	// send data to lcd
	LCD_PORT |= (1<<LCD_EN);	// turn on EN pin
	_delay_ms(1);
	LCD_PORT &= ~(1<<LCD_EN);	// turn EN pin back off
	_delay_ms(1);
}

/************************************************************************/
/* lcd_init()                                                           
 command sequence is: 0x03, wait 5ms, 0x03, wait 100us, 0x03,
 0x02, 0x02, 0x08, 0, 0x08, 0, 1, 0, 0x06
/************************************************************************/
void lcd_init(){
	LCD_REGISTER= 0xFF;	// set as output
	lcd_set_bits(0x0);
	_delay_ms(20);
	/* reset process from datasheet */
	lcd_cmd(0x03);
	_delay_ms(5);
	lcd_cmd(0x03);
	_delay_ms(11);
	lcd_cmd(0x03);
	/* end reset */
	lcd_cmd(0x02);
	lcd_cmd(0x02);
	lcd_cmd(0x08);
	lcd_cmd(0x0);
	lcd_cmd(0x0C);
	lcd_cmd(0x0);
	lcd_cmd(0x06);
}

/************************************************************************/
/* lcd_clear()
   just clears all characters off the display
/************************************************************************/
void lcd_clear(){
	lcd_cmd(0x0);
	lcd_cmd(0x01);
}

/************************************************************************/
/* lcd_write_char()                                                 
   writes on character to the current cursor position.
   Note: because we are using 4-bit communication, we have to send a char
   in 2 sections. We have to send the upper half first, then the lower.
/************************************************************************/
void lcd_write_char(uint8_t c){
	uint8_t upper_half, lower_half;
	upper_half = c & 0x0F;
	lower_half = c & 0xF0;
	LCD_SET_RS();
	lcd_set_bits(upper_half);	// transfer upper half to lcd
	LCD_SET_EN();
	_delay_ms(1);
	LCD_UNSET_EN();
	_delay_ms(1);
	lcd_set_bits(lower_half);	// transfer lower half to lcd
	LCD_SET_EN();
	_delay_ms(1);
	LCD_UNSET_EN();
	_delay_ms(1);
}