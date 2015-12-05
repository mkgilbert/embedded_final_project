#ifndef SPI_H
#define SPI_H

/* Preprocessor directives */
#define CS_ENABLE() (PORTB &= ~CS)
#define CS_DISABLE() (PORTB |= CS)
#define CS (1 << PB4)
#define MOSI (1 << PB5)
#define MISO (1 << PB6)
#define SCK (1 << PB7)
#define CS_DDR DDRB
#define CS_PORT PORTB
#define SPI_DDR DDRB
#define SPI_PORT PORTB

void spi_init (void);
unsigned char spi_rxtx (unsigned char data);

#endif /* !SPI_H */