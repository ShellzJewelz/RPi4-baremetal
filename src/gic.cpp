#include "hw_config.h"

#if RPI_VERSION == RPI_VERSION_4

#include "gic.h"

static void gic_distributor_init();
static void gic_controller_init();

void gic_init()
{
    gic_distributor_init();
    gic_controller_init();
}

void gic_timer_interrupt_init()
{
    // Each gicd_itargetsr register holds 4 bytes for the 4 * n-th interrupts
    // Each bit in the byte corresponds to a target cpu
    // Interrupt 30's target will be gicd_itargetsr register number 7 (30 / 4)
    // If the bit is 1 that cpu will receive that interrupt otherwise not
    gicd_itargetsr_n_t* gicd_itargetsr7 = (gicd_itargetsr_n_t*)GICD_ITARGETSR(7);
    gicd_itargetsr7->CPU_targets_offset_2B = GICD_ITARGETS_ENABLE_CPU0; // Targets for INTID 30 (EL1 Physical Timer)

    // Each gicd_ipriorityr register holds 4 priorities for the 4 * n-th interrupts
    // Priority of interrupt 30 will be in gicd_ipriorityr register number 7 (30 / 4)
    // Lower priority values mean higher priority
    gicd_ipriorityr_n_t* gicd_ipriority7 = (gicd_ipriorityr_n_t*)GICD_IPRIORITYR(7);
    gicd_ipriority7->Priority_offset_2B = 0; // Priority for INTID 30 (EL1 Physical Timer)

    // Each gicd_icfgr register holds 16 configurations for the 16 * n-th interrupts
    // Configuration if interrupt 30 will be in gicd_icfgr register number 1 (30 / 16)
    gicd_icfgr_n_t* gicd_icfgr1 = (gicd_icfgr_n_t*)GICD_ICFGR(1);
    gicd_icfgr1->Int_config14 = GICD_ICFGR_EDGE; // INTID 30 is edge triggered

    // Each gicd_isenabler register holds 32 enable bits for the 32 * n-th interrupts
    // Enable bit of interrupt 30 will be in gicd_isenabler register number 0 (30 / 32)
    gicd_isenabler_n_t* gicd_isenabler0 = (gicd_isenabler_n_t*)GICD_ISENABLER(0);
    *gicd_isenabler0 = (1 << 30); // Interrupt 30 is forwarded
}

void gic_timer_interrupt_handle()
{
    gicd_icactiver_n_t* gicd_icactiver0 = (gicd_icactiver_n_t*)GICD_ICACTIVER(0);
    *gicd_icactiver0 = (0 << 30); // Interrupt 30 is cleared
}

static void gic_distributor_init()
{
    gicd_ctlr_secure_t* gicd_ctlr = (gicd_ctlr_secure_t*)GIC_DISTRIBUTOR;
    gicd_ctlr->EnableGrp0 = 0;   // Secure Group 0 interrupts are disabled
    gicd_ctlr->EnableGrp1NS = 0; // Non-secure Group 1 interrupts are disabled
    gicd_ctlr->DS = 1;           // Non-secure accesses are permitted to access and modify registers
	gicd_ctlr->ARE_NS = 1;       // Afinity routing enabled for Non-secure state

    printf("\nGIC Distributor Information\n");

    gicd_iidr_t* gicd_iidr = (gicd_iidr_t*)GICD_IIDR;
    printf("Implementer: 0x%X (0x43B ARM)\n"
           "Revision: 0x%X (0x1)\n"
           "Variant: 0x%X (0x0)\n"
           "ProductID: 0x%X (0x02 GIC-400)\n", 
           gicd_iidr->Implementer,
           gicd_iidr->Revision,
           gicd_iidr->Variant,
           gicd_iidr->ProductID);

    // All available interrupts are non secure
	gicd_typer_t* gicd_typer = (gicd_typer_t*)GICD_TYPER;
	unsigned int implemented_interrupt_groups = gicd_typer->ITLinesNumber + 1;

    printf("ITLinesNumber: %d\n"
           "CPUNumber: %d (4 Cores)\n"
           "SecurityExtn: %d\n"
           "Lockable SPIs: %d\n", 
           gicd_typer->ITLinesNumber + 1,
           gicd_typer->CPUNumber + 1,
           gicd_typer->SecurityExtn,
           gicd_typer->LSPI);

    // Set all interrupts to be in non-secure Group 1
	for (unsigned int n = 0; n <= implemented_interrupt_groups; n++)
    {
        gicd_igroupr_n_t* gicd_igrouprn = (gicd_igroupr_n_t*)GICD_IGROUPR(n);
        *gicd_igrouprn = ~((unsigned int)0);
    }

	// Disable all interrupts
	for (unsigned int n = 0; n <= implemented_interrupt_groups; n++)
    {
        gicd_icenabler_n_t* gicd_icenablern = (gicd_icenabler_n_t*)GICD_ICENABLER(n);
        *gicd_icenablern = ~((unsigned int)0);
    }

	// Clear all pending interrupts
	for (unsigned int n = 0; n <= implemented_interrupt_groups; n++)
    {
        gicd_icpendr_n_t* gicd_icpendrn = (gicd_icpendr_n_t*)GICD_ICPENDR(n);
        *gicd_icpendrn = ~((unsigned int)0);
    }

    // Set all interrupts priority to the lowest
    for (unsigned int n = 0; n <= implemented_interrupt_groups * 8; n++)
    {
        gicd_ipriorityr_n_t* gicd_ipriorityrn = (gicd_ipriorityr_n_t*)GICD_IPRIORITYR(n);
        gicd_ipriorityrn->Priority_offset_0B = ((unsigned char)~(0));
        gicd_ipriorityrn->Priority_offset_1B = ((unsigned char)~(0));
        gicd_ipriorityrn->Priority_offset_2B = ((unsigned char)~(0));
        gicd_ipriorityrn->Priority_offset_3B = ((unsigned char)~(0));
    }

    gicd_ctlr->EnableGrp0 = 1;   // Secure Group 0 interrupts are enabled
    gicd_ctlr->EnableGrp1NS = 1; // Non-secure Group 1 interrupts are enabled
}

static void gic_controller_init()
{
    #if 0
    gicc_sre_el3_t icc_sre_el3;
    icc_sre_el3.SRE = 1;    // GIC ich_/icc_ system registers access in lower exception levels is enabled
    icc_sre_el3.DFB = 1;    // FIQ bypass is disabled
    icc_sre_el3.DIB = 1;    // IRQ bypass is disabled
    icc_sre_el3.Enable = 1; // Access to icc_sre_el1 and icc_sre_el2 in lower excpetion levels is enabled
    gic_set_icc_sre_el3(icc_sre_el3);

    gicc_ctlr_el3_t icc_ctlr_el3;
    icc_ctlr_el3.PMHE = 1;          // Priority mask hint is enabled
    icc_ctlr_el3.EOImode_EL1NS = 1; // icc_eoir0_el1 / icc_eoir1_el1 provide priority drop functionality, 
                                    // icc_dir_el1 provides deactivation functionality.
    gic_set_icc_ctlr_el3(icc_ctlr_el3);	

    gicc_ctlr_el1_t icc_ctlr_el1;
    icc_ctlr_el1.PMHE = 1;    // Priority mask hint is enabled
    icc_ctlr_el1.EOImode = 1; // icc_eoir0_el1 / icc_eoir1_el1 provide priority drop functionality, 
                              // icc_dir_el1 provides deactivation functionality.
    gic_set_icc_ctlr_el1(icc_ctlr_el1);

	gicc_pmr_el1_t gicc_pmr_el1;
	gicc_pmr_el1.Priority = (unsigned char)GICC_PMR_EL1_PRIORITY_ALL_VALUES;
	gic_set_icc_pmr_el1(gicc_pmr_el1);
    #endif

    gicc_ctlr_t* gicc_ctlr = (gicc_ctlr_t*)GICC_CTLR;
    gicc_ctlr->EnableGrp0 = 0;
    gicc_ctlr->EnableGrp1 = 0;

    printf("\nGIC CPU Interface Information (0x%X)\n", GIC_CPU_INTERFACE);

    gicc_iidr_t* gicc_iidr = (gicc_iidr_t*)GICC_IIDR;
    printf("Implementer: 0x%X (0x43B ARM)\n"
           "Revision: 0x%X (0x1)\n"
           "Architecture version: 0x%X (0x2 version 2.0)\n"
           "ProductID: 0x%X (0x020 GIC-400)\n\n", 
           gicc_iidr->Implementer,
           gicc_iidr->Revision,
           gicc_iidr->ArchitectureVersion,
           gicc_iidr->ProductID);

    gicc_pmr_t* gicc_pmr = (gicc_pmr_t*)GICC_PMR;
    gicc_pmr->Priority = 0xff; // Higher value means lower priority 

    gicc_bpr_t* gicc_bpr = (gicc_bpr_t*)GICC_BPR;
    gicc_bpr->Binary_Point = 0x0;

    #if 0
    gicc_iar_t* gicc_iar = (gicc_iar_t*)GICC_IAR;
    gicc_eoir_t* gicc_eoir = (gicc_eoir_t*)GICC_EOIR;

    // Clear all active interrupts
    for (gicc_iar_t active_interrupt = *gicc_iar; 
        active_interrupt.INTID != GICC_IAR_SPURIOUS_INTID; 
        active_interrupt = *gicc_iar)
    {
        printf("INTID %d\n", active_interrupt.INTID);
        // Clear current active interrupt
        *gicc_eoir = active_interrupt; 
    }
    printf("Cleared all active interrupts\n");
    #endif

    #if 0
	gicc_igrpen0_el1_t icc_igrpen0_el1;
	icc_igrpen0_el1.Enable = 1; // Group 0 interrupts are enabled
	gic_set_icc_igrpen0_el1(icc_igrpen0_el1);

	gicc_igrpen1_el1_t icc_igrpen1_el1;
    icc_igrpen1_el1.Enable = 1; // 	Group 1 interrupts are enabled
    gic_set_icc_igrpen1_el1(icc_igrpen1_el1);

    printf("Enabled All Groups\n");
    #endif

    gicc_ctlr->EnableGrp0 = 1;
    gicc_ctlr->EnableGrp1 = 1;
}

#endif // RPI_VERSION == RPI_VERSION_4