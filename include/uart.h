#ifndef UART_H
#define UART_H

void uart_init(unsigned int baud_rate);
void uart_putc(unsigned char c);
char uart_getc();
void uart_puts(const char* s);
void uart_puti(long int number, int base=10);
void uart_putmem(const unsigned char* src, unsigned int size, int base);

#endif // UART_H