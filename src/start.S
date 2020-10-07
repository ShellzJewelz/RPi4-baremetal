#include "system_registers.h"

.section ".text.boot"

.global _start

_start:
	// read cpu id, stop slave cores
	// important to keep x0 value for kernel_main
	mrs x0, mpidr_el1
	and x0, x0, #3
	cbz x0, master

// cpu id > 0, stop
hang:
	wfe
	b hang

master:
/*
	ldr	x0, =SCTLR_VALUE_MMU_DISABLED
	msr	sctlr_el1, x0		

	ldr	x0, =HCR_VALUE
	msr	hcr_el2, x0

	ldr	x0, =SCR_VALUE
	msr	scr_el3, x0

	ldr	x0, =SPSR_VALUE
	msr	spsr_el3, x0

	adr	x0, el1_entry		
	msr	elr_el3, x0

	eret			
el1_entry:
*/

// set stack before our code
setup_stack:
	ldr x1, =_stack
	mov sp, x1

	// clear bss
	ldr x1, =__bss_start
	ldr w2, =__bss_size

clear_bss_loop:
	cbz w2, kernel_entry
	str xzr, [x1], #8
	sub w2, w2, #1
	cbnz w2, clear_bss_loop

// jump to C code, should not return
kernel_entry:
	bl kernel_main
	// for failsafe, halt this core too
	b hang
