#include "hw_config.h"
#include "uart.h"
#include "mem.h"
#include "frame_buffer.h"
#include "utils.h"

extern "C" void kernel_main(unsigned long int cpu_id)
{
    uart_init(115200);
    mem_init();
    frame_buffer_init();

    frame_buffer_puts("Exception Level: ");

    unsigned long int el = get_el();
    frame_buffer_putc((const unsigned char)el + '0');

    while (true) 
    {
        uart_putc(uart_getc());
    }
}