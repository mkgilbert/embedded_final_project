#include <avr/io.h>
#include <stdio.h>
#include "spi.h"

void spi_init (void) {
    CS_DDR |= CS; // Set CS as an output
    SPI_DDR |= MOSI | SCK; // Set MOSI and SCK as outputs
    SPI_PORT |= MISO; // pullup in MISO, might not be needed

    //SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPR1);
    SPCR = (1 << SPE) | (1 << MSTR);
    
    char i;
    
    // ]r:10
    CS_DISABLE();
    for(i=0; i<10; i++) // idle for 1 bytes / 80 clocks
        spi_rxtx(0xFF);
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