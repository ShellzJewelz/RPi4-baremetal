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

/**
 * Wait N microsec (ARM CPU only)
 */
void timer_wait_msec(unsigned int n)
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

/**
 * Get System Timer's counter
 */
unsigned long int timer_get_system_time()
{
    unsigned int timer_high = -1;
    unsigned int timer_low = -1;

    // we must read MMIO area as two separate 32 bit reads
    timer_high = *SYSTEM_TIMER_HIGH;
    timer_low = *SYSTEM_TIMER_LOW;

    // we have to repeat it if high word changed during read
    if(timer_high != *SYSTEM_TIMER_HIGH) 
    {
        timer_high = *SYSTEM_TIMER_HIGH;
        timer_low = *SYSTEM_TIMER_LOW;
    }
    
    // compose long int value
    return ((unsigned long int) timer_high << 32) | timer_low;
}

/**
 * Wait N microsec (with BCM System Timer)
 */
void timer_wait_msec_st(unsigned int n)
{
    unsigned long int system_time = timer_get_system_time();
    // we must check if it's non-zero, because qemu does not emulate
    // system timer, and returning constant zero would mean infinite loop
    if (system_time)
    {
        while (timer_get_system_time() < system_time + n)
        {

        }
    }
}