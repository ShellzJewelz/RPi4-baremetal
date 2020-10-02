#ifndef UART_H
#define UART_H

void uart_init();
void uart_send(unsigned int c);
char uart_recv();
void uart_send_string(const char* s);
void uart_send_int(int number, int base);

#endif // UART_H