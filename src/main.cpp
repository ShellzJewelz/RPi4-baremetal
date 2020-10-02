#include "hw_config.h"
#include "uart.h"
#include "mem.h"

int main()
{
    uart_init();
    mem_init();

    while (true) 
    {
    }
    
    return 0;
}