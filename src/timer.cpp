#include "timer.h"
#include "hw_config.h"

/**
 * Wait N CPU cycles (ARM CPU only)
 */
void timer_wait_cycles(unsigned int n)
{
    for (; n > 0; n--)
    {
        asm volatile("nop");
    }
}

void timer_wait_usec(unsigned int n)
{
    register unsigned long frequency, end_system_time, current_system_time;
    // get the current counter frequency
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(frequency));
    // read the current counter
    asm volatile ("mrs %0, cntpct_el0" : "=r"(end_system_time));
    // calculate expire value for counter
    end_system_time += ((frequency / 1000) * n)/ 1000;
    do
    {
        asm volatile ("mrs %0, cntpct_el0" : "=r"(current_system_time));
    } while(current_system_time < end_system_time);
}

void timer_wait_msec(unsigned int n)
{
    timer_wait_usec(n * 1000);
}

/**
 * Get System Timer's counter
 */
unsigned long int timer_get_system_time()
{
    unsigned int timer_high = -1;
    unsigned int timer_low = -1;

    // we must read MMIO area as two separate 32 bit reads
    timer_high = *TIMER_HIGH;
    timer_low = *TIMER_LOW;

    // we have to repeat it if high word changed during read
    if(timer_high != *TIMER_HIGH) 
    {
        timer_high = *TIMER_HIGH;
        timer_low = *TIMER_LOW;
    }
    
    // compose long int value
    return ((unsigned long int) timer_high << 32) | timer_low;
}