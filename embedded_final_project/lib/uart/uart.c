/*
 * Name: uart.c
 * Author: Brandon Garling
 */
#include "system.h"
#include "uart.h"
#include <util/setbaud.h>
#include <avr/io.h>
#include <stdio.h>

void uart_putchar(char c, FILE *stream) {
    if (c == '\n') {
        uart_putchar('\r', stream);
    }
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = c;
}

char uart_getchar(FILE *stream) {
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

void uart_init(void) {
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;
    
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif
    
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */

    static FILE uart_stream = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
    
    stdout = stdin = &uart_stream;
}

void uart_print_buffer(uint8_t * buffer, uint16_t len, uint8_t seperate_at){
    printf("\n");
    uint16_t i;
    for(i=0; i<len; i++) {
        printf(" %02X", buffer[i]);
        if ((i + 1) % seperate_at == 0){
            printf(" | ");
            uint16_t j;
            for (j = i - seperate_at + 1; j < i + 1; j++){
                if (buffer[j] >= 32 && buffer[j] <= 126){
                    printf("%c", buffer[j]);
                }
                else{
                    printf(".");
                }
            }
            printf("\n");
        }
    }
    printf("\n");
}
