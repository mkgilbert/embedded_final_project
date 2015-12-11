#ifndef SPI_H
#define SPI_H

/* Preprocessor directives */
#define CS_ENABLE(PRT) (PORT_UNSET(PRT))
#define CS_DISABLE(PRT) (PORT_SET(PRT))

#define SPI_MOSI_PORT B
#define SPI_MOSI_PIN 5

#define SPI_MISO_PORT B
#define SPI_MISO_PIN 6

#define SPI_SCK_PORT B
#define SPI_SCK_PIN 7

void spi_init (void);
unsigned char spi_rxtx (unsigned char data);

#endif /* !SPI_H */