#include "hw_config.h"
#include "uart.h"
#include "mem.h"
#include "frame_buffer.h"
#include "utils.h"
#include "interrupt_controller.h"
#include "scheduler.h"
#include "timer.h"

extern "C" void kernel_main(unsigned long int cpu_id)
{
    uart_init(115200);
    mem_init();
    frame_buffer_init();
    interrupt_controller_init();
    scheduler_init();

    interrupt_controller_irqs_enable();

    frame_buffer_puts("Exception Level: ");

    unsigned long int el = get_el();
    frame_buffer_putc((const unsigned char)el + '0');
    frame_buffer_putc('\n');

    frame_buffer_puts("Timer test: ");
    timer_wait_usec(200000);
    frame_buffer_puts("200000\n");

    while (true) 
    {
        uart_putc(uart_getc());
    }
}