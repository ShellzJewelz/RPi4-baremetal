#include "exceptions.h"
#include "frame_buffer.h"
#include "uart.h"

const char* entry_error_messages[] = 
{
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",		
	"FIQ_INVALID_EL1t",		
	"ERROR_INVALID_EL1T",		

	"SYNC_INVALID_EL1h",		
	"IRQ_INVALID_EL1h",		
	"FIQ_INVALID_EL1h",		
	"ERROR_INVALID_EL1h",		

	"SYNC_INVALID_EL0_64",		
	"IRQ_INVALID_EL0_64",		
	"FIQ_INVALID_EL0_64",		
	"ERROR_INVALID_EL0_64",	

	"SYNC_INVALID_EL0_32",		
	"IRQ_INVALID_EL0_32",		
	"FIQ_INVALID_EL0_32",		
	"ERROR_INVALID_EL0_32"	
};

extern "C" void exceptions_invalid_entry_message(int type, unsigned long esr, unsigned long address)
{
	frame_buffer_puts(entry_error_messages[type]);
	frame_buffer_putc('\n');
	/*
    uart_puts(entry_error_messages[type]);
    uart_puts(" ESR: ");
    uart_puti(esr, 16);
    uart_puts(" Address: ");
    uart_puti(address, 16);
    uart_putc('\n');
	*/
}