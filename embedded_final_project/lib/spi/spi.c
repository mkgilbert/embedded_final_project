#include "system.h"
#include <avr/io.h>
#include "lib/port_helpers/port_helpers.h"
#include "spi.h"

void spi_init (void) {
	// Set CS, MOSI, and SCK as outputs
	PORT_OUTPUT (SPI_MOSI);
	PORT_OUTPUT (SPI_SCK);
	
	// Set MISO as an input and enable the pull-up resistor
	PORT_INPUT (SPI_MISO);
	PORT_SET (SPI_MISO);

	// Enable the SPI interrupts and set this device as master
	// Note: This runs SPI at F_CPU/4
	
    SPCR = (1 << SPE) | (1 << MSTR);
}

//TX to slave device AND RX from slave AT SAME TIME (same function for
//both RX and TX. I.E. Must TX dummy byte to RX from slave
unsigned char spi_rxtx (unsigned char data) {
    // Load data into the buffer
    SPDR = data;
    //Wait until transmission complete
    while(!(SPSR & (1<<SPIF) ));
    // Return received data
    return(SPDR);
}