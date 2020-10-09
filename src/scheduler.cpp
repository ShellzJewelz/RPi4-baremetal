#include "scheduler.h"
#include "uart.h"
#include "frame_buffer.h"
#include "hw_config.h"
#include "utils.h"
#include "interrupt_controller.h"

#define TIMER_CONTROL_STATUS_MASK0	(1 << 0)
#define TIMER_CONTROL_STATUS_MASK1	(1 << 1)
#define TIMER_CONTROL_STATUS_MASK2	(1 << 2)
#define TIMER_CONTROL_STATUS_MASK3	(1 << 3)

const unsigned int g_interval = 200000;
unsigned int g_current_compare_register = 0;

static void inline scheduler_update_timer_compare_register();

void scheduler_init()
{
	g_current_compare_register = read32(TIMER_LOW);
	g_current_compare_register += g_interval;
	write32(TIMER_COMPARE_REGISTER1, g_current_compare_register);
	interrupt_controller_irqs1_enable(INTERRUPT_CONTROLLER_IRQS1_TIMER_1, scheduler_yield);
}

void scheduler_yield() 
{
	scheduler_update_timer_compare_register();
	// Clear timer compare 1 interrupt 
	write32(TIMER_CONTROL_STATUS, TIMER_CONTROL_STATUS_MASK1);
	frame_buffer_puts("Scheduler yielded\n");
	//uart_puts("Timer interrupt received\n");
}

static void inline scheduler_update_timer_compare_register()
{
	g_current_compare_register += g_interval;
	write32(TIMER_COMPARE_REGISTER1, g_current_compare_register);
}