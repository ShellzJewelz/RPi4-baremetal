#include "system_registers.h"

.section ".text.boot"

.global _start

_start:
	// Read cpu id, stop slave cores
	mrs x0, mpidr_el1
	and x0, x0, #3
	cbz x0, master

	// Core id > 0, stop
hang:
	wfe
	b hang

master:
	// Set stack before our code
	ldr x1, =_stack

    // Set up EL1
    mrs x0, CurrentEL
    and x0, x0, #0xC // clear reserved bits

    // Running at EL3?
el3_entry:
    cmp x0, #0xC
    bne el2_entry
    // Change to EL2
    mov x2, #SCR_EL3_VALUE
	msr scr_el3, x2
    mov x2, #SPSR_EL3_VALUE
    msr spsr_el3, x2
    adr x2, el2_entry
    msr elr_el3, x2
    eret

el2_entry:
    // running at EL2?
	cmp x0, #4
    beq el1_entry
    msr sp_el1, x1
    // enable CNTP for EL1
    mrs x0, cnthctl_el2
    orr x0, x0, #CNTHCTL_EL2_VALUE
    msr cnthctl_el2, x0
    msr cntvoff_el2, xzr
    // enable AArch64 in EL1
    mov x0, #(1 << 31)
	orr x0, x0, #(1 << 1)
    msr hcr_el2, x0
    mrs x0, hcr_el2
    // change execution level to EL1
    mov x2, #0x1C5
    msr spsr_el2, x2
    adr x2, el1_entry
    msr elr_el2, x2
    eret

el1_entry:
	mov x0, #(3 << 28) 
	orr x0, x0, #(3 << 22)
	orr x0, x0, #(1 << 20)
	orr x0, x0, #(1 << 11)
	msr	sctlr_el1, x0

// set stack before our code
setup_stack:
	mov sp, x1

	// clear bss
	ldr x1, =__bss_start
	ldr w2, =__bss_size

clear_bss_loop:
	cbz w2, kernel_entry
	str xzr, [x1], #8
	sub w2, w2, #1
	cbnz w2, clear_bss_loop

kernel_entry:
	bl GlobalInitialize
	mrs x0, mpidr_el1
	and x0, x0, #3
	bl kernel_main
	// for failsafe, halt this core too
	b hang
