.section ".text.boot"

.global _start

_start:
// read cpu id, stop slave cores
mrs x1, mpidr_el1
and x1, x1, #3
cbz x1, setup_stack

// cpu id > 0, stop
hang:
wfe
b hang

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
bl main
// for failsafe, halt this core too
b hang

.global memzero
memzero:
	str xzr, [x0], #8
	subs x1, x1, #8
	b.gt memzero
	ret

.global write32
write32:
	str w1,[x0]
	ret

.global read32
read32:
	ldr w0,[x0]
	ret
