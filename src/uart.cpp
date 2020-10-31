#include "hw_config.h"
#include "timer.h"
#include "utils.h"

/**
 * Set baud rate and characteristics (8N1) and map to GPIO
 */
void uart_init(unsigned int baud_rate)
{
    register unsigned int r;

    /* Initialize UART */
    // Enable UART1, UART1 mini uart
    write32(UART1_ENABLE, 1);
    // Disable interrupts
    write32(UART1_MU_IER, 0);
    // Disable transmitter and receiver
    write32(UART1_MU_CNTL, 0);
    // Set to 8 bits mode
    write32(UART1_MU_LCR, 3);
    // Disable RTS
    write32(UART1_MU_MCR, 0);
    // Enable FIFO, Clear FIFO
    write32(UART1_MU_IIR, 0xC6);
    r = ((GPU_CORE_FREQUENCY / baud_rate) / 8) - 1;
    write32(UART1_MU_BAUD, r);

    /* map UART1 to GPIO pins */
    r = read32(GPIO_GPFSEL1);
    // GPIO 14
    r &= ~(7 << 12); 
    // Set GPIO 14 to alternative function 5
    r |= (2 << 12); 
    // GPIO 15
    r &= (7 << 15); 
    // Set GPIO 15 to alternative function 5
    r |= (2 << 15); 
    write32(GPIO_GPFSEL1, r);

    // PullUD disable
    write32(GPIO_GPPUD, 0); 
    timer_wait_cycles(150);
    // Enable GPIO pins 14 and 15
    write32(GPIO_GPPUDCLK0, (1 << 14) | (1 << 15));
    timer_wait_cycles(150);
    // Flush GPIO setup
    write32(GPIO_GPPUDCLK0, 0);   
     
    // Enable transmitter and receiver
    write32(UART1_MU_CNTL, 3);
}

/**
 * Send a character
 */
void uart_putc(const unsigned char c) 
{
    /* wait until we can send */
    do
    {
        timer_wait_single_cycle();
    } while (!(read32(UART1_MU_LSR) & (1 << 5)));
    /* write the character to the buffer */
    write32(UART1_MU_IO, c);
}

void uart_putc(void*, const char c)
{
    uart_putc((const unsigned char)c);
}

/**
 * Receive a character
 */
char uart_getc() 
{
    char r;
    /* wait until something is in the buffer */
    do
    {
        timer_wait_single_cycle();
    } while (!(read32(UART1_MU_LSR) & 0x01));
    /* read it and return */
    r = (char)(read32(UART1_MU_IO));
    /* convert carrige return to newline */
    return r == 'r' ? '\n' : r;
}

/**
 * Display a string
 */
void uart_puts(const char* s) 
{
    while(*s) 
    {
        /* convert newline to carrige return + newline */
        if(*s == '\n')
        {
            uart_putc('\r');
        }

        uart_putc(*s);
        s++;
    }
}