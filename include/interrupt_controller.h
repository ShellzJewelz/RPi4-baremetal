#ifndef INTERRUPT_CONTROLLER_H
#define INTERRUPT_CONTROLLER_H

typedef enum interrupt_controller_irqs1
{
    INTERRUPT_CONTROLLER_IRQS1_TIMER_0 = 0,
    INTERRUPT_CONTROLLER_IRQS1_TIMER_1,
    INTERRUPT_CONTROLLER_IRQS1_TIMER_2,
    INTERRUPT_CONTROLLER_IRQS1_TIMER_3,
} interrupt_controller_irqs1_e;

typedef void (*irq_handler_function)();

void interrupt_controller_init();
void interrupt_controller_irqs_enable();
void interrupt_controller_irqs_disable();
void interrupt_controller_irqs1_enable(interrupt_controller_irqs1_e irq, irq_handler_function handler);
void interrupt_controller_irqs1_disable(interrupt_controller_irqs1 irq);

#endif // INTERRUPT_CONTROLLER_H