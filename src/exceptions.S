#define EXCEPTIONS_STACK_FRAME_SIZE		(256) // size of all saved registers 

#define EXCEPTIONS_SYNC_INVALID_EL1t    (0) 
#define EXCEPTIONS_IRQ_INVALID_EL1t	    (1) 
#define EXCEPTIONS_FIQ_INVALID_EL1t	    (2) 
#define EXCEPTIONS_ERROR_INVALID_EL1t   (3) 

#define EXCEPTIONS_SYNC_INVALID_EL1h    (4) 
#define EXCEPTIONS_IRQ_INVALID_EL1h	    (5) 
#define EXCEPTIONS_FIQ_INVALID_EL1h	    (6) 
#define EXCEPTIONS_ERROR_INVALID_EL1h   (7) 

#define EXCEPTIONS_SYNC_INVALID_EL0_64	(8)
#define EXCEPTIONS_IRQ_INVALID_EL0_64	(9)
#define EXCEPTIONS_FIQ_INVALID_EL0_64   (10) 
#define EXCEPTIONS_ERROR_INVALID_EL0_64	(11) 

#define EXCEPTIONS_SYNC_INVALID_EL0_32	(12) 
#define EXCEPTIONS_IRQ_INVALID_EL0_32	(13) 
#define EXCEPTIONS_FIQ_INVALID_EL0_32	(14) 
#define EXCEPTIONS_ERROR_INVALID_EL0_32	(15) 

#define EXCEPTIONS_SYNC_INVALID_EL2t    (16)
#define EXCEPTIONS_IRQ_INVALID_EL2t     (17)
#define EXCEPTIONS_FIQ_INVALID_EL2t     (18)
#define EXCEPTIONS_ERROR_INVALID_EL2t   (19)

#define EXCEPTIONS_SYNC_INVALID_EL2h    (20)
#define EXCEPTIONS_IRQ_INVALID_EL2h     (21)
#define EXCEPTIONS_FIQ_INVALID_EL2h     (22)
#define EXCEPTIONS_ERROR_INVALID_EL2h   (23)

#define EXCEPTIONS_SYNC_INVALID_EL1_64  (24)
#define EXCEPTIONS_IRQ_INVALID_EL1_64   (25)
#define EXCEPTIONS_FIQ_INVALID_EL1_64   (26)
#define EXCEPTIONS_ERROR_INVALID_EL1_64 (27)

#define EXCEPTIONS_SYNC_INVALID_EL1_32  (28)
#define EXCEPTIONS_IRQ_INVALID_EL1_32   (29)
#define EXCEPTIONS_FIQ_INVALID_EL1_32   (30)
#define EXCEPTIONS_ERROR_INVALID_EL1_32 (31)

#define EXCEPTIONS_SYNC_INVALID_EL3t    (32)
#define EXCEPTIONS_IRQ_INVALID_EL3t     (33)
#define EXCEPTIONS_FIQ_INVALID_EL3t     (34)
#define EXCEPTIONS_ERROR_INVALID_EL3t   (35)

#define EXCEPTIONS_SYNC_INVALID_EL3h    (36)
#define EXCEPTIONS_IRQ_INVALID_EL3h     (37)
#define EXCEPTIONS_FIQ_INVALID_EL3h     (38)
#define EXCEPTIONS_ERROR_INVALID_EL3h   (39)

#define EXCEPTIONS_SYNC_INVALID_EL2_64  (40)
#define EXCEPTIONS_IRQ_INVALID_EL2_64   (41)
#define EXCEPTIONS_FIQ_INVALID_EL2_64   (42)
#define EXCEPTIONS_ERROR_INVALID_EL2_64 (43)

#define EXCEPTIONS_SYNC_INVALID_EL2_32  (44)
#define EXCEPTIONS_IRQ_INVALID_EL2_32   (45)
#define EXCEPTIONS_FIQ_INVALID_EL2_32   (46)
#define EXCEPTIONS_ERROR_INVALID_EL2_32 (47)

.macro kernel_entry
	sub	sp, sp, #EXCEPTIONS_STACK_FRAME_SIZE
	stp	x0, x1, [sp, #16 * 0]
	stp	x2, x3, [sp, #16 * 1]
	stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]
	str	x30, [sp, #16 * 15] 
.endm

.macro kernel_exit
	ldp	x0, x1, [sp, #16 * 0]
	ldp	x2, x3, [sp, #16 * 1]
	ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]
	ldr	x30, [sp, #16 * 15] 
	add	sp, sp, #EXCEPTIONS_STACK_FRAME_SIZE		
	eret
.endm

.macro handle_invalid_entry_el1 type
	msr spsel, #0
	kernel_entry
	mov	x0, #\type
	mrs	x1, esr_el1
	mrs	x2, elr_el1
	bl exceptions_invalid_entry_message
	b err_hang
.endm

.macro handle_invalid_entry_el2 type
	msr spsel, #0
	kernel_entry
	mov	x0, #\type
	mrs	x1, esr_el2
	mrs	x2, elr_el2
	bl exceptions_invalid_entry_message
	b err_hang
.endm

.macro handle_invalid_entry_el3 type
	msr spsel, #0
	kernel_entry
	mov	x0, #\type
	mrs	x1, esr_el3
	mrs	x2, elr_el3
	bl exceptions_invalid_entry_message
	b err_hang
.endm

.macro ventry label
.align 7
	b \label
.endm

/*
 * Exception vectors.
 */
.align 11
.global vectors_el1
vectors_el1:
	ventry sync_invalid_el1t			// Synchronous EL1t
	ventry irq_el1t				        // IRQ EL1t
	ventry fiq_invalid_el1t				// FIQ EL1t
	ventry error_invalid_el1t			// Error EL1t 

	ventry sync_invalid_el1h			// Synchronous EL1h
	ventry irq_invalid_el1h			    // IRQ EL1h
	ventry fiq_invalid_el1h				// FIQ EL1h
	ventry error_invalid_el1h			// Error EL1h

	ventry sync_invalid_el0_64			// Synchronous 64-bit EL0
	ventry irq_invalid_el0_64			// IRQ 64-bit EL0
	ventry fiq_invalid_el0_64			// FIQ 64-bit EL0
	ventry error_invalid_el0_64			// Error 64-bit EL0

	ventry sync_invalid_el0_32			// Synchronous 32-bit EL0
	ventry irq_invalid_el0_32			// IRQ 32-bit EL0
	ventry fiq_invalid_el0_32			// FIQ 32-bit EL0
	ventry error_invalid_el0_32			// Error 32-bit EL0

sync_invalid_el1t:
	handle_invalid_entry_el1 EXCEPTIONS_SYNC_INVALID_EL1t

irq_el1t:
	kernel_entry 
	bl exceptions_el1_irq_handler
	kernel_exit 

fiq_invalid_el1t:
	handle_invalid_entry_el1 EXCEPTIONS_FIQ_INVALID_EL1t

error_invalid_el1t:
	handle_invalid_entry_el1 EXCEPTIONS_ERROR_INVALID_EL1t

sync_invalid_el1h:
	handle_invalid_entry_el1 EXCEPTIONS_SYNC_INVALID_EL1h

irq_invalid_el1h:
	handle_invalid_entry_el1 EXCEPTIONS_IRQ_INVALID_EL1h

fiq_invalid_el1h:
	handle_invalid_entry_el1 EXCEPTIONS_FIQ_INVALID_EL1h

error_invalid_el1h:
	handle_invalid_entry_el1 EXCEPTIONS_ERROR_INVALID_EL1h

sync_invalid_el0_64:
	handle_invalid_entry_el1 EXCEPTIONS_SYNC_INVALID_EL0_64

irq_invalid_el0_64:
	handle_invalid_entry_el1 EXCEPTIONS_IRQ_INVALID_EL0_64

fiq_invalid_el0_64:
	handle_invalid_entry_el1 EXCEPTIONS_FIQ_INVALID_EL0_64

error_invalid_el0_64:
	handle_invalid_entry_el1 EXCEPTIONS_ERROR_INVALID_EL0_64

sync_invalid_el0_32:
	handle_invalid_entry_el1 EXCEPTIONS_SYNC_INVALID_EL0_32

irq_invalid_el0_32:
	handle_invalid_entry_el1 EXCEPTIONS_IRQ_INVALID_EL0_32

fiq_invalid_el0_32:
	handle_invalid_entry_el1 EXCEPTIONS_FIQ_INVALID_EL0_32

error_invalid_el0_32:
	handle_invalid_entry_el1 EXCEPTIONS_ERROR_INVALID_EL0_32

.align 11
.global vectors_el2
vectors_el2:
	ventry sync_invalid_el2t			// Synchronous EL2t
	ventry irq_invalid_el2t				// IRQ EL2t
	ventry fiq_invalid_el2t				// FIQ EL2t
	ventry error_invalid_el2t			// Error EL2t 

	ventry sync_invalid_el2h			// Synchronous EL2h
	ventry irq_invalid_el2h				// IRQ EL2h
	ventry fiq_invalid_el2h				// FIQ EL2h
	ventry error_invalid_el2h			// Error EL2h

	ventry sync_invalid_el1_64			// Synchronous 64-bit EL1
	ventry irq_invalid_el1_64			// IRQ 64-bit EL1
	ventry fiq_invalid_el1_64			// FIQ 64-bit EL1
	ventry error_invalid_el1_64			// Error 64-bit EL1

	ventry sync_invalid_el1_32			// Synchronous 32-bit EL1
	ventry irq_invalid_el1_32			// IRQ 32-bit EL1
	ventry fiq_invalid_el1_32			// FIQ 32-bit EL1
	ventry error_invalid_el1_32			// Error 32-bit EL1

sync_invalid_el2t:
	handle_invalid_entry_el2 EXCEPTIONS_SYNC_INVALID_EL2t

irq_invalid_el2t:
	handle_invalid_entry_el2 EXCEPTIONS_IRQ_INVALID_EL2t

fiq_invalid_el2t:
	handle_invalid_entry_el2 EXCEPTIONS_FIQ_INVALID_EL2t

error_invalid_el2t:
	handle_invalid_entry_el2 EXCEPTIONS_ERROR_INVALID_EL2t

sync_invalid_el2h:
	handle_invalid_entry_el2 EXCEPTIONS_SYNC_INVALID_EL2h

irq_invalid_el2h:
	handle_invalid_entry_el2 EXCEPTIONS_IRQ_INVALID_EL2h

fiq_invalid_el2h:
	handle_invalid_entry_el2 EXCEPTIONS_FIQ_INVALID_EL2h

error_invalid_el2h:
	handle_invalid_entry_el2 EXCEPTIONS_ERROR_INVALID_EL2h

sync_invalid_el1_64:
	handle_invalid_entry_el2 EXCEPTIONS_SYNC_INVALID_EL1_64

irq_invalid_el1_64:
	handle_invalid_entry_el2 EXCEPTIONS_IRQ_INVALID_EL1_64

fiq_invalid_el1_64:
	handle_invalid_entry_el2 EXCEPTIONS_FIQ_INVALID_EL1_64

error_invalid_el1_64:
	handle_invalid_entry_el2 EXCEPTIONS_ERROR_INVALID_EL1_64

sync_invalid_el1_32:
	handle_invalid_entry_el2 EXCEPTIONS_SYNC_INVALID_EL1_32

irq_invalid_el1_32:
	handle_invalid_entry_el2 EXCEPTIONS_IRQ_INVALID_EL1_32

fiq_invalid_el1_32:
	handle_invalid_entry_el2 EXCEPTIONS_FIQ_INVALID_EL1_32

error_invalid_el1_32:
	handle_invalid_entry_el2 EXCEPTIONS_ERROR_INVALID_EL1_32

.align 11
.global vectors_el3
vectors_el3:
	ventry sync_invalid_el3t			// Synchronous EL3t
	ventry irq_invalid_el3t				// IRQ EL3t
	ventry fiq_invalid_el3t				// FIQ EL3t
	ventry error_invalid_el3t			// Error EL3t 

	ventry sync_invalid_el3h			// Synchronous EL3h
	ventry irq_invalid_el3h				// IRQ EL3h
	ventry fiq_invalid_el3h				// FIQ EL3h
	ventry error_invalid_el3h			// Error EL3h

	ventry sync_invalid_el2_64			// Synchronous 64-bit EL2
	ventry irq_invalid_el2_64			// IRQ 64-bit EL2
	ventry fiq_invalid_el2_64			// FIQ 64-bit EL2
	ventry error_invalid_el2_64			// Error 64-bit EL2

	ventry sync_invalid_el2_32			// Synchronous 32-bit EL2
	ventry irq_invalid_el2_32			// IRQ 32-bit EL2
	ventry fiq_invalid_el2_32			// FIQ 32-bit EL2
	ventry error_invalid_el2_32			// Error 32-bit EL2

sync_invalid_el3t:
	handle_invalid_entry_el3 EXCEPTIONS_SYNC_INVALID_EL3t

irq_invalid_el3t:
	handle_invalid_entry_el3 EXCEPTIONS_IRQ_INVALID_EL3t

fiq_invalid_el3t:
	handle_invalid_entry_el3 EXCEPTIONS_FIQ_INVALID_EL3t

error_invalid_el3t:
	handle_invalid_entry_el3 EXCEPTIONS_ERROR_INVALID_EL3t

sync_invalid_el3h:
	handle_invalid_entry_el3 EXCEPTIONS_SYNC_INVALID_EL3h

irq_invalid_el3h:
	handle_invalid_entry_el3 EXCEPTIONS_IRQ_INVALID_EL3h

fiq_invalid_el3h:
	handle_invalid_entry_el3 EXCEPTIONS_FIQ_INVALID_EL3h

error_invalid_el3h:
	handle_invalid_entry_el3 EXCEPTIONS_ERROR_INVALID_EL3h

sync_invalid_el2_64:
	handle_invalid_entry_el3 EXCEPTIONS_SYNC_INVALID_EL2_64

irq_invalid_el2_64:
	handle_invalid_entry_el3 EXCEPTIONS_IRQ_INVALID_EL2_64

fiq_invalid_el2_64:
	handle_invalid_entry_el3 EXCEPTIONS_FIQ_INVALID_EL2_64

error_invalid_el2_64:
	handle_invalid_entry_el3 EXCEPTIONS_ERROR_INVALID_EL2_64

sync_invalid_el2_32:
	handle_invalid_entry_el3 EXCEPTIONS_SYNC_INVALID_EL2_32

irq_invalid_el2_32:
	handle_invalid_entry_el3 EXCEPTIONS_IRQ_INVALID_EL2_32

fiq_invalid_el2_32:
	handle_invalid_entry_el3 EXCEPTIONS_FIQ_INVALID_EL2_32

error_invalid_el2_32:
	handle_invalid_entry_el3 EXCEPTIONS_ERROR_INVALID_EL2_32

.global err_hang
err_hang: 
    b err_hang

.global exceptions_init
exceptions_init:
	adr	x0, vectors_el1	// load VBAR_EL1 with virtual
	msr	vbar_el1, x0	// vector table address
	adr	x0, vectors_el2	// load VBAR_EL2 with virtual
	msr	vbar_el2, x0	// vector table address
	adr	x0, vectors_el3	// load VBAR_EL3 with virtual
	msr	vbar_el3, x0	// vector table address	
	ret

.global exceptions_irq_enable
exceptions_irq_enable:
	msr daifclr, #2 
	ret

.global exceptions_irq_disable
exceptions_irq_disable:
	msr	daifset, #2
	ret
