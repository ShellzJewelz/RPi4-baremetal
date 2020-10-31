#ifndef UART_H
#define UART_H

void uart_init(unsigned int baud_rate);
void uart_putc(const unsigned char c);
void uart_putc(void*, const char c);
char uart_getc();
void uart_puts(const char* s);

#endif // UART_H