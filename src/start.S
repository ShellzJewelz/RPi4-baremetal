.section ".text.boot"

.global _start
_start:
	// Read cpu id, stop slave cores
	mrs x0, mpidr_el1
	and x0, x0, #3
	cbz x0, master

	// Core id > 0, stop
hang:
	// wfe
	b hang

master:
	// Use the current stack for all exception levels
	msr spsel, #0

setup_stack:
	// Set stack before our code
	ldr x1, =_stack
	mov sp, x1

bss_init:
	// Clear bss
	ldr x1, =__bss_start
	ldr w2, =__bss_size

clear_bss_loop:
	cbz w2, low_level_init
	str xzr, [x1], #8
	sub w2, w2, #1
	cbnz w2, clear_bss_loop

low_level_init:
	bl kernel_init
	adr x0, kernel_entry
	msr elr_el3, x0
	eret

kernel_entry:
	bl kernel_main
	// For failsafe, halt this core too
	b hang
