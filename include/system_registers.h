#ifndef SYSTEM_REGISTERS_H
#define SYSTEM_REGISTERS_H

// SPSR_EL3, Saved Program Status Register

#define SPSR_EL3_D (0 << 9) // Debug exceptions are not masked
#define SPSR_EL3_A (1 << 8) // SError interrupts are masked
#define SPSR_EL3_I (1 << 7) // IRQ interrupts are masked
#define SPSR_EL3_F (1 << 6) // FIQ interrupts are masked
#define SPSR_EL3_RES0 (0 << 5)
#define SPSR_EL3_AARCH64 (0 << 4) // Always 0 (Aarch64) 
#define SPSR_EL3_M (9 << 0) // EL2h (EL2 with our own dedicated stack)
#define SPSR_EL3_VALUE (SPSR_EL3_M | SPSR_EL3_AARCH64 | SPSR_EL3_RES0 | SPSR_EL3_F | \
					    SPSR_EL3_I | SPSR_EL3_A | SPSR_EL3_D) 

// SCR_EL3, Secure Configuration Register

#define SCR_EL3_RW (1 << 10) // Execution state for lower level will be AArch64
#define SCR_EL3_SIF (1 << 9) // Disable Secure Instruction Fetch
#define SCR_EL3_HCE (1 << 8) // HCE instructions are enabled
#define SCR_EL3_SMD (0 << 7) // SMC instructions are enabled
#define SCR_EL3_RES (3 << 4) 
#define SCR_EL3_EA (0 << 3)  // Don't take External aborts and SError interrupts
#define SCR_EL3_FIQ (0 << 2) // Don't take FIQ interrupts
#define SCR_EL3_IRQ (0 << 1) // Don't take IRQ interrupts
#define SCR_EL3_NS (1 << 0)  // Enable Non-secure state
#define SCR_EL3_VALUE (SCR_EL3_NS | SCR_EL3_IRQ | SCR_EL3_FIQ | SCR_EL3_EA | SCR_EL3_RES | \
					   SCR_EL3_SMD | SCR_EL3_HCE | SCR_EL3_SIF | SCR_EL3_RW) 

// CNTHCTL_EL2 - Counter-timer Hypervisor Control register

#define CNTHCTL_EL2_EL1PCEN (1 << 1)  // Don't trap physical timer register access from EL0 and EL1
#define CNTHCTL_EL2_EL1PCTEN (1 << 0) // Don't trap physical counter register access from EL0 and EL1
#define CNTHCTL_EL2_VALUE (CNTHCTL_EL2_EL1PCTEN | CNTHCTL_EL2_EL1PCEN)

// HCR_EL2, Hypervisor Configuration Register

#define HCR_EL2_RW (1 << 31)  // Execution state for EL1 is AArach64
#define HCR_EL2_SWIO (1 << 0) // Data cache invalide by set/way instructions
#define HCR_EL2_VALUE (HCR_EL2_SWIO | HCR_EL2_RW)

// SPSR_EL2, Saved Program Status Register

#define SPSR_EL2_D (0 << 9) // Debug exceptions are not masked
#define SPSR_EL2_A (1 << 8) // SError interrupts are masked
#define SPSR_EL2_I (1 << 7) // IRQ interrupts are masked
#define SPSR_EL2_F (1 << 6) // FIQ interrupts are masked
#define SPSR_EL2_RES0 (0 << 5)
#define SPSR_EL2_AARCH64 (0 << 4) // Always 0 (Aarch64) 
#define SPSR_EL2_M (5 << 0) // EL1h (EL1 with our own dedicated stack)
#define SPSR_EL2_VALUE (SPSR_EL2_M | SPSR_EL2_AARCH64 | SPSR_EL2_RES0 | SPSR_EL2_F | \
					    SPSR_EL2_I | SPSR_EL2_A | SPSR_EL2_D) 

// SCTLR_EL1, System Control Register (EL1)

#define SCTLR_RES1 (3 << 28) | (3 << 22) | (1 << 20) | (1 << 11)
#define SCTLR_EL1_ENDA (0 << 27) // Pointer authentication of data addresses is disabled
#define SCTLR_EL1_UCI (0 << 26) // Execution of the cache maintenance instructions at EL0 are trapped
#define SCTLR_EL1_EE (0 << 25) // Explicit data accesses at EL1 are little-endian
#define SCTLR_EL1_EOE (0 << 24) // Explicit data accesses at EL0 are little-endian
#define SCTLR_EL1_NTWE (0 << 18) // EL0 WFE instruction traps are enabled
#define SCTLR_EL1_RES0 (0 << 17) 
#define SCTLR_EL1_NTWI (0 << 16) // EL0 WFI instruction traps are enabled
#define SCTLR_EL1_ENDB (0 << 13) // Pointer authentication of data addresses is disabled
#define SCTLR_EL1_I (0 << 12) // Instruction cache is disabled
// #define SCTLR_EL1_EOS (1 << 11) // Exception return from EL1 is not a context synchronizing event
#define SCTLR_EL1_UMA (0 << 9) // EL0 Access to DAIF is trapped
#define SCTLR_EL1_NAA (0 << 6) // Non-Aligned Access faults generation AT EL1 and EL0 are enabled
#define SCTLR_EL1_SA0 (0 << 4) // Stack alignment checks for EL0 are disabled
#define SCTLR_EL1_SA (0 << 3) // Stack alignment checks are disabled
#define SCTLR_EL1_C (0 << 2) // Data chache is disabled
#define SCTLR_EL1_A (0 << 1) // Alignment checks are disabled
#define SCTLR_EL1_M (0 << 0) // MMU is disabled

#define SCTLR_EL1_VALUE (SCTLR_EL1_M | SCTLR_EL1_A | SCTLR_EL1_C | SCTLR_EL1_SA | SCTLR_EL1_SA0 | \
                         SCTLR_EL1_NAA | SCTLR_EL1_UMA | SCTLR_EL1_I | SCTLR_EL1_ENDB | SCTLR_EL1_NTWI  | \
                         SCTLR_EL1_RES0 | SCTLR_EL1_NTWE | SCTLR_EL1_EOE | SCTLR_EL1_EE | SCTLR_EL1_UCI | \
                         SCTLR_EL1_ENDA | SCTLR_RES1)

#define CPACR_EL1_FPEN (3 << 20) // SVE, Advanced SIMD, and floating-point registers are enabled
#define CPACR_EL1_VALUE (CPACR_EL1_FPEN)

#endif // SYSTEM_REGISTERS_H
