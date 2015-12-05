#ifndef UART_H
#define UART_H

#define UART_SUPPORT

#define BAUD 57700

void uart_init(void);
void uart_print_buffer(uint8_t * buffer, uint16_t len, uint8_t seperate_at);

#endif /* !UART_H */