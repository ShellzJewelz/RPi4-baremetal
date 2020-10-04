#ifndef UART_H
#define UART_H

void uart_init();
void uart_putc(unsigned char c);
char uart_getc();
void uart_puts(const char* s);
void uart_puti(int number, int base);

#endif // UART_H