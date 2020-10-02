#ifndef TIMER_H
#define TIMER_H

/**
 * Wait Single CPU cycle (ARM CPU only)
 */
inline void timer_wait_single_cycle()
{
    asm volatile("nop");
}

void timer_wait_cycles(unsigned int n);
void timer_wait_msec(unsigned int n);
unsigned long int timer_get_system_timer();
void timer_wait_msec_st(unsigned int n);

#endif // TIMER_H