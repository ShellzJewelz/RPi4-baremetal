#include "hw_config.h"
#include "uart.h"
#include "mem.h"
extern "C" void kernel_main(unsigned long int cpu_id)
{
    uart_init(115200);
    mem_init();

    while (true) 
    {
        uart_putc(uart_getc());
    }
}