#include "interrupt_controller.h"
#include "exceptions.h"
#include "uart.h"
#include "frame_buffer.h"
#include "hw_config.h"
#include "utils.h"

const int INTERRUPT_CONTROLLER_IRQ_TABLE_SIZE = 32;

static irq_handler_function g_irqs1_handlers[INTERRUPT_CONTROLLER_IRQ_TABLE_SIZE] = { nullptr };
static irq_handler_function g_irqs2_handlers[INTERRUPT_CONTROLLER_IRQ_TABLE_SIZE] = { nullptr };
static irq_handler_function g_basic_irqs_handlers[INTERRUPT_CONTROLLER_IRQ_TABLE_SIZE] = { nullptr }; 

void interrupt_controller_init()
{
    exceptions_vector_init();
}

void interrupt_controller_irqs_enable()
{
    exceptions_irq_enable();
}

void interrupt_controller_irqs_disable()
{
    exceptions_irq_disable();
}

void interrupt_controller_irqs1_enable(interrupt_controller_irqs1_e irq, irq_handler_function handler)
{
    g_irqs1_handlers[irq] = handler;
    unsigned int irq_enable_mask = BIT(irq);
    unsigned int enable_irqs_1 = read32(ENABLE_IRQS_1);
    write32(ENABLE_IRQS_1, enable_irqs_1 | irq_enable_mask);
}

void interrupt_controller_irqs1_disable(interrupt_controller_irqs1_e irq)
{
    
}

extern "C" void interrupt_controller_handle_irqs()
{
    frame_buffer_puts("Interrupt Occurred\n");

	unsigned int irq = read32(IRQ_PENDING_1);
    unsigned int irq_handler = 0;

	switch (irq) 
    {
		case (INTERRUPT_CONTROLLER_IRQS1_TIMER_0):
            irq_handler = INTERRUPT_CONTROLLER_IRQS1_TIMER_0;
			break;
        case (INTERRUPT_CONTROLLER_IRQS1_TIMER_1):
            irq_handler = INTERRUPT_CONTROLLER_IRQS1_TIMER_1;
            break;
        case (INTERRUPT_CONTROLLER_IRQS1_TIMER_2):
            irq_handler = INTERRUPT_CONTROLLER_IRQS1_TIMER_2;
            break;
        case (INTERRUPT_CONTROLLER_IRQS1_TIMER_3):
            irq_handler = INTERRUPT_CONTROLLER_IRQS1_TIMER_3;
            break;
	}

    if (irq_handler != 0)
    {
        if (g_irqs1_handlers[irq_handler] != nullptr)
        {
            g_irqs1_handlers[irq_handler]();
        }
    }
    else
    {
        frame_buffer_puts("Unknown pending irq\n");
        /*
        uart_puts("Unknown pending irq: ");
        uart_puti(irq, 16);
        uart_putc('\n');
        */
    }
}