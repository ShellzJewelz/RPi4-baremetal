#include "hw_config.h"
#include "system_registers.h"
#include "exceptions.h"
#include "utils.h"

#if RPI_VERSION == RPI_VERSION_4
#include "gic.h"
#endif // RPI_VERSION == RPI_VERSION_4

extern "C" void kernel_init()
{
    cptr_el3_t cptr_el3;
    cptr_el3.EZ = 1;  // Accesses to SVE are not trapped
    cptr_el3.TFP = 0; // Accesses to SVE, Advanced SIMD and floating-point are not trapped
    exception_level3_set_cptr(cptr_el3);

    scr_el3_t scr_el3;
    scr_el3.NS = 1;  // Enable Non-secure state
    scr_el3.IRQ = 0; // Don't take IRQ interrupts
    scr_el3.IFQ = 0; // Don't take FIQ interrupts
    scr_el3.EA = 0;  // Don't take External aborts and SError interrupts
    scr_el3.SMD = 0; // SMC instructions are enabled
    scr_el3.HCE = 1; // HCE instructions are enabled
    scr_el3.SIF = 1; // Disable Secure Instruction Fetch
    scr_el3.RW = 1;  // Execution state for lower level will be AArch64
    exception_level3_set_scr(scr_el3);

    hcr_el2_t hcr_el2;
    hcr_el2.VM = 0;   // EL1&0 stage 2 address translation disabled
    hcr_el2.FMO = 0;  // Physical FIQ interrupts are not taken to EL2
    hcr_el2.IMO = 0;  // Physical IRQ interrupts are not taken to EL2
    hcr_el2.AMO = 0;  // Physical SError interrupts are not taken to EL2
    hcr_el2.TWI = 0;  // WFI instruction at EL0 or EL1 are not trapped
    hcr_el2.TWE = 0;  // WFE instruction at EL0 or EL1 are not trapped
    hcr_el2.TSC = 1;  // SMC instruction at EL1 are trapped
    hcr_el2.TACR = 0; // Access to the Auxiliary Control Registers to EL2 are trapped
    hcr_el2.HCD = 0;  // HVC instruction at EL2 and EL1 are enabled
    hcr_el2.SWIO = 1; // Data cache invalide by set/way instructions
    hcr_el2.RW = 1;   // Execution state for EL1 is AArach64
    exception_level2_set_hcr(hcr_el2);

    cnthctl_el2_t cnthctl_el2 = exception_level2_get_cnthctl_el2();
    cnthctl_el2.EL1PCEN = 1;  // Don't trap physical timer register access from EL0 and EL1
    cnthctl_el2.EL1PCTEN = 1; // Don't trap physical counter register access from EL0 and EL1
    exception_level2_set_cnthctl_el2(cnthctl_el2);    

    sctlr_el1_t sctlr_el1;
    sctlr_el1.M = 0;    // MMU is disabled
    sctlr_el1.A = 1;    // Alignment checks are enabled
    sctlr_el1.C = 0;    // Data chache is disabled
    sctlr_el1.SA = 1;   // Stack alignment checks are enabled
    sctlr_el1.SA0 = 1;  // Stack alignment checks for EL0 are enabled
    sctlr_el1.nAA = 0;  // Non-Aligned Access faults generation AT EL1 and EL0 are enabled
    sctlr_el1.UMA = 0;  // EL0 Access to DAIF is trapped
    sctlr_el1.I = 0;    // Instruction cache is disabled
    sctlr_el1.nTWI = 0; // EL0 WFI instruction traps are enabled
    sctlr_el1.nTWE = 0; // EL0 WFE instruction traps are enabled
    sctlr_el1.EOE = 0;  // Explicit data accesses at EL0 are little-endian
    sctlr_el1.EE = 0;   // Explicit data accesses at EL1 are little-endian
    sctlr_el1.UCI = 0;  // Execution of the cache maintenance instructions at EL0 are trapped
    exception_level1_set_sctlr(sctlr_el1);

    cpacr_el1_t cpacr_el1;
    cpacr_el1.FPEN = CPACR_EL1_FPEN_EL0_EL1_NOT_TRAPPED; // SVE, Advanced SIMD, and floating-point registers are enabled
    exception_level1_set_cpacr(cpacr_el1);

    spsr_el3_t spsr_el3;
    spsr_el3.M = SPSR_EL3_M_EL1t; // EL1 with current stack
    spsr_el3.M_AARCH64 = 0;       // Always 0 (Aarch64) 
    spsr_el3.F = 0;               // FIQ interrupts are not masked
    spsr_el3.I = 0;               // IRQ interrupts are not masked
    spsr_el3.A = 0;               // SError interrupts are not masked
    spsr_el3.D = 0;               // Debug exceptions are not masked
    exception_level3_set_spsr(spsr_el3);

    #if DEBUG_HARDWARE == DEBUG_HARDWARE_DISPLAY_HARDWARE
    frame_buffer_init();
    puts("[ OK ] Frame Buffer init\n");
    init_printf(nullptr, frame_buffer_printf_putc);
    puts("[ OK ] Printf init\n");
    #elif DEBUG_HARDWARE == DEBUG_HARDWARE_VIRTUAL_HARDWARE 
    puts("[ OK ] UART init\n");
    init_printf(nullptr, uart_printf_putc);
    puts("[ OK ] Printf init\n");
    #endif // DEBUG_HARDWARE == DEBUG_HARDWARE_DISPLAY_HARDWARE

    exceptions_init();
    printf("[ OK ] Exceptions init\n");
    #if RPI_VERSION == RPI_VERSION_4
    gic_init();
    printf("[ OK ] GIC init\n");
    #endif // RPI_VERSION == RPI_VERSION_4
}

extern "C" void kernel_main()
{
    printf("\nException Level: %u\n", get_el());

    #if 0
    exceptions_irq_enable();

    #if RPI_VERSION == RPI_VERSION_4
    gic_timer_interrupt_init();
    printf("Timer test\n");
    #endif // RPI_VERSION == RPI_VERSION_4
    #endif
    
    while (true) 
    {
    }
}