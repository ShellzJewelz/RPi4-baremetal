#include "hw_config.h"
#include "uart.h"
#include "mem.h"
#include "frame_buffer.h"

extern "C" void kernel_main(unsigned long int cpu_id)
{
    uart_init(115200);
    mem_init();
    frame_buffer_init();

    frame_buffer_print(80, 80, "Hello World!\n");

    while (true) 
    {
        uart_putc(uart_getc());
    }
}