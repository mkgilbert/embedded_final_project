#include "system.h"
#include <avr/io.h>
#include <util/delay.h>
#include "lib/port_helpers/port_helpers.h"
#include "lib/spi/spi.h"
#include "sdcard.h"

int8_t sd_init() {
	uint8_t i;

	// Set our SD_CS port as an output
	PORT_OUTPUT (SD_CS);
	
	// Deselect our SD card and idle for 80 clock cycles
	CS_DISABLE (SD_CS);
	for(i=0; i<100; i++)
		spi_rxtx(0xFF);

	// Go to the idle state using CMD0
	for(i=0; i<10 && sd_command(SD_GO_IDLE_STATE, 0x00000000, 8) != 1; i++)
		_delay_ms(100);
	
	if(i == 10) // SD card did not enter idle state
		return -1;

	// Tell the SD card what modes we support and the operating voltage
	// using CMD8.
	// Note: This command requires a correct CRC7
	sd_command(SD_SEND_IF_COND, 0x400001aa, 8);

	// Bring the SD card out of idle using ACMD41, retry a maximum 10 times
	for (i = 0; i < 10; i++) {
		// Send CMD55 first to let the SD card know that the following 
		// command is an application specific command.
		sd_command(SD_APP_CMD, 0x00000000, 8);
		// Send ACMD41 and check to see if we got out of idle or not
		if (sd_command(SD_ACMD_SEND_OP_COND, 0x40000000, 8) == 0)
			break;
		_delay_ms(100);
	}
	
	if(i == 10) // SD card did not come out of idle
		return -2;
	
	// Set the block length using CMD16
	// Probably not needed on SDHC cards, but just in case
	sd_command(SD_SET_BLOCKLEN, 0x00000200, 8);	
	
	// Everything went as expected, return 0
	return 0;
}

//sd_crc7(&data, length)
//
// Calculates and returns the CRC-7 for use with SD cards
//
// Source: http://www.microchip.com/forums/tm.aspx?m=94767&mpage=1&key=CRC7%F0%97%AA%B8
//
uint8_t sd_crc7 (uint8_t *data, uint8_t length)
{
	uint8_t i, bit, c, crc;
	crc = 0x00; // Set initial value
	
	for (i = 0; i < length; i++, data++)
	{
		c = *data;
		
		for (bit = 0; bit < 8; bit++)
		{
			crc = crc << 1;
			if ((c ^ crc) & 0x80)
				crc = crc ^ 0x09;
			c = c << 1;
		}
		
		crc = crc & 0x7F;
	}
	
	crc = (crc << 1) | (0x01);
	return crc;
}

uint8_t sd_command(uint8_t cmd, uint32_t arg, uint16_t read) {
    uint16_t i;
	uint8_t buffer_in[read];
	uint8_t buffer_out[SD_COMMAND_LEN];
	
    CS_ENABLE(SD_CS);
	
	buffer_out[0] = cmd | 0x40;
	buffer_out[1] = arg >> 24;
	buffer_out[2] = arg >> 16;
	buffer_out[3] = arg >> 8;
	buffer_out[4] = arg;
	buffer_out[5] = sd_crc7(buffer_out, 5);

	for (i=0; i<6; i++)
		spi_rxtx(buffer_out[i]);
	    
    for(i=0; i<read; i++)
        buffer_in[i] = spi_rxtx(0xFF);
	
    CS_DISABLE(SD_CS);
    	
    return buffer_in[1];
}

void sd_read(uint32_t sector, uint16_t offset, uint8_t * buffer,
             uint16_t len) {

    uint16_t i = 0;

	// Bypass the sd_command function and the CRC calculation to gain some speed
    CS_ENABLE(SD_CS);

    spi_rxtx(SD_READ_SINGLE_BLOCK | 0x40);
    spi_rxtx(sector >> 24);
    spi_rxtx(sector >> 16);
    spi_rxtx(sector >> 8);
    spi_rxtx(sector);
    spi_rxtx(0xFE);
    	
    for(i=0; i<SD_RW_WAIT_RETRIES && spi_rxtx(0xFF) != 0x00; i++) {} // wait for 0
    
    for(i=0; i<SD_RW_WAIT_RETRIES && spi_rxtx(0xFF) != 0xFE; i++) {} // wait for data start
	
    for(i=0; i<offset; i++) // "skip" bytes
        spi_rxtx(0xFF);
    
    for(i=0; i<len; i++) // read len bytes
        buffer[i] = spi_rxtx(0xFF);
    
    for(i+=offset; i<512; i++) // "skip" again
        spi_rxtx(0xFF);
    
    // skip checksum
    spi_rxtx(0xFF);
    spi_rxtx(0xFF);
	
	// wait for the device to become available again
	for (i=0; i<SD_RW_WAIT_RETRIES && spi_rxtx(0xFF) != 0xFF; i++) {};
    
    CS_DISABLE(SD_CS);
}

void sd_write(uint32_t sector, uint8_t * buffer) {
    /* write a single block to sd card - len should ALWAYS be 512 */
    uint16_t i = 0;
    uint16_t len = 512;
    
    CS_ENABLE(SD_CS);
    
    spi_rxtx(SD_WRITE_SINGLE_BLOCK | 0x40);
    spi_rxtx(sector>>24); // sector >> 24
    spi_rxtx(sector>>16); // sector >> 16
    spi_rxtx(sector>>8); // sector >> 8
    spi_rxtx(sector); // sector
    spi_rxtx(0xFF);        // CRC
    
    for(i=0; i<10 && spi_rxtx(0xFF) != 0x00; i++) {} // wait for 0
    //if (i == 10){
        //printf("%sDid not receive 0x0\n%s", C_RED, C_WHITE);
    //}
    
    // sending data must begin with a start token of 0xFE
    for(i=0; i<10 && (spi_rxtx(0xFE) & 0x05) != 0x05; i++) {}      // response should be 0bxxx00101 format (which is 5)
    //if (i == 10){
        //printf("%sDid not receive 0x05\n%s", C_RED, C_WHITE);
    //}
    
    for(i=0; i<len; i++) // send len bytes
        spi_rxtx(buffer[i]);
    
    // skip checksum
    spi_rxtx(0xFF);
    spi_rxtx(0xFF);

    //uint8_t response = spi_rxtx(0xFF);
    
    //if (response == 0xE5){
        //printf("%Bytes written successfully\n%s", C_RED, C_WHITE);
    //}
    
    CS_DISABLE(SD_CS);
}
