/* lcd commands
   By: Mike Gilbert */

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
		LCD_PORT |= (0<<LCD_DB4);
	if (hex_value & 2)
		LCD_PORT |= (1<<LCD_DB5);
	else
		LCD_PORT |= (0<<LCD_DB5);
	if (hex_value & 4)
		LCD_PORT |= (1<<LCD_DB6);
	else
		LCD_PORT |= (0<<LCD_DB6);
	if (hex_value & 8)
		LCD_PORT |= (1<<LCD_DB7);
	else
		LCD_PORT |= (0<<LCD_DB7);
}

/************************************************************************/
/* lcd_cmd()
   sends the lcd a command as listed in the command table on the datasheet
/************************************************************************/
void lcd_cmd(uint8_t hex_value){
	LCD_PORT &= (0<<LCD_RS);	// turn off RS pin
	lcd_set_bits(hex_value);	// send data to lcd
	LCD_PORT |= (1<<LCD_EN);	// turn on EN pin
	_delay_ms(1);
	LCD_PORT &= (0<<LCD_EN);	// turn EN pin back off
	_delay_ms(1);
}

/************************************************************************/
/* lcd_init()                                                           
 command sequence is: 0x03, wait 5ms, 0x03, wait 100us, 0x03,
 0x02, 0x02, 0x08, 0, 0x08, 0, 1, 0, 0x06
/************************************************************************/
void lcd_init(){
	LCD_PORT |= 0xFF;	// set all pins as output
	lcd_cmd(0x03);
	_delay_ms(5);
	lcd_cmd(0x03);
	_delay_ms(1);
	lcd_cmd(0x03);
	lcd_cmd(0x02);
	lcd_cmd(0x02);
	lcd_cmd(0x08);
	lcd_cmd(0x0);
	lcd_cmd(0x08);
	lcd_cmd(0x0);
	lcd_cmd(0x01);
	lcd_cmd(0x0);
	lcd_cmd(0x06);
}