#ifndef GIC_H
#define GIC_H

#include "hw_config.h"

#ifndef GIC_BASE
#if RPI_VERSION == RPI_VERSION_4
#ifdef HIGH_PERI
#define GIC_BASE           ((void*)(0x4C0040000UL))
#else
#define GIC_BASE           ((void*)(0xFF840000))
#endif // HIGH_PERI
#else
#error "Unsupported Raspberry Pi GIC"
#endif // RPI_VERSION == RPI_VERSION_4
#endif // GIC_BASE

#define GIC_DISTRIBUTOR    ((void*)((unsigned char*)GIC_BASE + 0x1000))
#define GIC_CPU_INTERFACE  ((void*)((unsigned char*)GIC_BASE + 0x2000))

#define GICD_CTLR          ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0000))
#define GICD_TYPER         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0004))
#define GICD_IIDR          ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0008))
#define GICD_IGROUPRn      ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0080))
#define GICD_ISENABLERn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0100))
#define GICD_ICENABLERn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0180))
#define GICD_ISPENDRn      ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0200))
#define GICD_ICPENDRn      ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0280))
#define GICD_ISACTIVERn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0300))
#define GICD_ICACTIVERn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0380))
#define GICD_IPRIORITYRn   ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0400))
#define GICD_ITARGETSRn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0800))
#define GICD_ICFGRn        ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0C00))
#define GICD_PPSIR         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0D00))
#define GICD_SPISRn        ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0D04))
#define GICD_SGIR          ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0F00))
#define GICD_CPENDSGIRn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0F10))
#define GICD_SPENDSGIRn    ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0F20))
#define GICD_PIDR4         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FD0))
#define GICD_PIDR5         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FD4))
#define GICD_PIDR6         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FD8))
#define GICD_PIDR7         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FDC))
#define GICD_PIDR0         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FE0))
#define GICD_PIDR1         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FE4))
#define GICD_PIDR2         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FE8))
#define GICD_PIDR3         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FEC))
#define GICD_CIDR0         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FF0))
#define GICD_CIDR1         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FF4))
#define GICD_CIDR2         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FF8))
#define GICD_CIDR3         ((unsigned int*)((unsigned char*)GIC_DISTRIBUTOR + 0x0FFC))

#define GICD_IGROUPR(n)    ((unsigned int*)((unsigned char*)GICD_IGROUPRn + (n * 0x4)))
#define GICD_ISENABLER(n)  ((unsigned int*)((unsigned char*)GICD_ISENABLERn + (n * 0x4)))
#define GICD_ICENABLER(n)  ((unsigned int*)((unsigned char*)GICD_ICENABLERn + (n * 0x4)))
#define GICD_ISPENDR(n)    ((unsigned int*)((unsigned char*)GICD_ISPENDRn + (n * 0x4)))
#define GICD_ICPENDR(n)    ((unsigned int*)((unsigned char*)GICD_ICPENDRn + (n * 0x4)))
#define GICD_ISACTIVER(n)  ((unsigned int*)((unsigned char*)GICD_ISACTIVERn + (n * 0x4)))
#define GICD_ICACTIVER(n)  ((unsigned int*)((unsigned char*)GICD_ICACTIVERn + (n * 0x4)))
#define GICD_IPRIORITYR(n) ((unsigned int*)((unsigned char*)GICD_IPRIORITYRn + (n * 0x4)))
#define GICD_ITARGETSR(n)  ((unsigned int*)((unsigned char*)GICD_ITARGETSRn + (n * 0x4)))
#define GICD_ICFGR(n)      ((unsigned int*)((unsigned char*)GICD_ICFGRn + (n * 0x4)))
#define GICD_SPISR(n)      ((unsigned int*)((unsigned char*)GICD_SPISRn + (n * 0x4)))
#define GICD_CPENDSGIR(n)  ((unsigned int*)((unsigned char*)GICD_CPENDSGIRn + (n * 0x4)))
#define GICD_SPENDSGIR(n)  ((unsigned int*)((unsigned char*)GICD_SPENDSGIRn + (n * 0x4)))

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int EnableGrp0 : 1;
    unsigned int EnableGrp1NS : 1;
    unsigned int EnableGrp1S : 1;
    unsigned int RESERVED0 : 1;
    unsigned int ARE_S : 1;
    unsigned int ARE_NS : 1;
    unsigned int DS : 1;
    unsigned int E1NWF : 1;
    unsigned int RESERVED1 : 23;
    unsigned int RWP : 1;
#else
    unsigned int RWP : 1;
    unsigned int RESERVED1 : 23;
    unsigned int E1NWF : 1;
    unsigned int DS : 1;
    unsigned int ARE_NS : 1;
    unsigned int ARE_S : 1;
    unsigned int RESERVED0 : 1;
    unsigned int EnableGrp1S : 1;
    unsigned int EnableGrp1NS : 1;
    unsigned int EnableGrp0 : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE       
} gicd_ctlr_secure_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int EnableGrp1 : 1;
    unsigned int EnableGrp1A : 1;
    unsigned int RESERVED0 : 2;
    unsigned int ARE_NS : 1;
    unsigned int RESERVED1 : 26;
    unsigned int RWP : 1;
#else
    unsigned int RWP : 1;
    unsigned int RESERVED1 : 26;
    unsigned int ARE_NS : 1;
    unsigned int RESERVED0 : 2;
    unsigned int EnableGrp1A : 1;
    unsigned int EnableGrp1 : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE       
} gicd_ctlr_non_secure_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
	unsigned int ITLinesNumber : 5;
	unsigned int CPUNumber : 3;
	unsigned int RESERVED0 : 2;
	unsigned int SecurityExtn : 1;
	unsigned int LSPI : 5;
	unsigned int RESERVED1 : 16;
#else
	unsigned int RESERVED1 : 16;
	unsigned int LSPI : 5;
	unsigned int SecurityExtn : 1;
	unsigned int RESERVED0 : 2;
	unsigned int CPUNumber : 3;
	unsigned int ITLinesNumber : 5;
#endif // ENDIANNESS == ENDIANNESS_LITTLE
} gicd_typer_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
	unsigned int Implementer : 12;
	unsigned int Revision : 4;
	unsigned int Variant : 4;
	unsigned int RESERVED0 : 4;
	unsigned int ProductID : 8;
#else
	unsigned int ProductID : 8;
	unsigned int RESERVED0 : 4;
	unsigned int Variant : 4;
	unsigned int Revision : 4;
	unsigned int Implementer : 12;
#endif // ENDIANNESS == ENDIANNESS_LITTLE
} gicd_iidr_t;

typedef unsigned int gicd_igroupr_n_t;
typedef unsigned int gicd_isenabler_n_t;
typedef unsigned int gicd_icenabler_n_t;
typedef unsigned int gicd_ispendr_n_t;
typedef unsigned int gicd_icpendr_n_t;
typedef unsigned int gicd_isactiver_n_t;
typedef unsigned int gicd_icactiver_n_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int Priority_offset_0B : 8;
    unsigned int Priority_offset_1B : 8;
    unsigned int Priority_offset_2B : 8;
    unsigned int Priority_offset_3B : 8;
#else
    unsigned int Priority_offset_3B : 8;
    unsigned int Priority_offset_2B : 8;
    unsigned int Priority_offset_1B : 8;
    unsigned int Priority_offset_0B : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicd_ipriorityr_n_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int CPU_targets_offset_0B : 8;
    unsigned int CPU_targets_offset_1B : 8;
    unsigned int CPU_targets_offset_2B : 8;
    unsigned int CPU_targets_offset_3B : 8;
#else
    unsigned int CPU_targets_offset_3B : 8;
    unsigned int CPU_targets_offset_2B : 8;
    unsigned int CPU_targets_offset_1B : 8;
    unsigned int CPU_targets_offset_0B : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicd_itargetsr_n_t;

#define BIT(x) (0b1 << x)
#define GICD_ITARGETS_ENABLE_CPU0 BIT(0)
#define GICD_ITARGETS_ENABLE_CPU1 BIT(1)
#define GICD_ITARGETS_ENABLE_CPU2 BIT(2)
#define GICD_ITARGETS_ENABLE_CPU3 BIT(3)
#define GICD_ITARGETS_ENABLE_CPU4 BIT(4)
#define GICD_ITARGETS_ENABLE_CPU5 BIT(5)
#define GICD_ITARGETS_ENABLE_CPU6 BIT(6)
#define GICD_ITARGETS_ENABLE_CPU7 BIT(7)

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int Int_config0 : 2;
    unsigned int Int_config1 : 2;
    unsigned int Int_config2 : 2;
    unsigned int Int_config3 : 2;
	unsigned int Int_config4 : 2;
	unsigned int Int_config5 : 2;
	unsigned int Int_config6 : 2;
	unsigned int Int_config7 : 2;
	unsigned int Int_config8 : 2;
	unsigned int Int_config9 : 2;
	unsigned int Int_config10 : 2;
	unsigned int Int_config11 : 2;
	unsigned int Int_config12 : 2;
	unsigned int Int_config13 : 2;
	unsigned int Int_config14 : 2;
	unsigned int Int_config15 : 2;
#else
    unsigned int Int_config15 : 2;
	unsigned int Int_config14 : 2;
	unsigned int Int_config13 : 2;
	unsigned int Int_config12 : 2;
	unsigned int Int_config11 : 2;
	unsigned int Int_config10 : 2;
	unsigned int Int_config9 : 2;
	unsigned int Int_config8 : 2;
	unsigned int Int_config7 : 2;
	unsigned int Int_config6 : 2;
	unsigned int Int_config5 : 2;
	unsigned int Int_config4 : 2;
	unsigned int Int_config3 : 2;
	unsigned int Int_config2 : 2;
	unsigned int Int_config1 : 2;
	unsigned int Int_config0 : 2;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicd_icfgr_n_t;

typedef enum
{
	GICD_ICFGR_LEVEL = 0b00,
	GICD_ICFGR_EDGE = 0b01
} gicd_icfgr_n_e;

typedef unsigned int gicd_spisr_n_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int SGI_clear_pending_bits0 : 8;
    unsigned int SGI_clear_pending_bits1 : 8;
    unsigned int SGI_clear_pending_bits2 : 8;
    unsigned int SGI_clear_pending_bits3 : 8;
#else
    unsigned int SGI_clear_pending_bits3 : 8;
    unsigned int SGI_clear_pending_bits2 : 8;
    unsigned int SGI_clear_pending_bits1 : 8;
    unsigned int SGI_clear_pending_bits0 : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicd_cpendsgir_n_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int SGI_set_pending_bits0 : 8;
    unsigned int SGI_set_pending_bits1 : 8;
    unsigned int SGI_set_pending_bits2 : 8;
    unsigned int SGI_set_pending_bits3 : 8;
#else
    unsigned int SGI_set_pending_bits3 : 8;
    unsigned int SGI_set_pending_bits2 : 8;
    unsigned int SGI_set_pending_bits1 : 8;
    unsigned int SGI_set_pending_bits0 : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicd_spendsgir_n_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
	unsigned int RESERVED0 : 8;
	struct
	{
		unsigned char VirtualMaintance : 1;
		unsigned char HypervisorTimer : 1;
		unsigned char VirtualTimer : 1;
		unsigned char nLegacyFIQ : 1;
		unsigned char SecurePhysicalTimer : 1;
		unsigned char NonsecurePhysicalTimer : 1;
		unsigned char nLegacyRIQ : 1;
	} __attribute__((packed)) PPIStatus;
	unsigned int RESERVED1 : 17;
#else
	unsigned int RESERVED1 : 17;
	struct
	{
		unsigned char nLegacyRIQ : 1;
		unsigned char NonsecurePhysicalTimer : 1;
		unsigned char SecurePhysicalTimer : 1;
		unsigned char nLegacyFIQ : 1;
		unsigned char VirtualTimer : 1;
		unsigned char HypervisorTimer : 1;
		unsigned char VirtualMaintance : 1;
	} __attribute__((packed)) PPIStatus;
	unsigned int RESERVED0 : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE
} gicd_ppisr_t;


#define GICC_CTLR          ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0000))
#define GICC_PMR           ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0004))
#define GICC_BPR           ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0008))
#define GICC_IAR           ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x000C))
#define GICC_EOIR          ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0010))
#define GICC_RPR           ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0014))
#define GICC_HPPIR         ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0018))
#define GICC_ABPR          ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x001C))
#define GICC_AIAR          ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0020))
#define GICC_AEOIR         ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0024))
#define GICC_AHPPIR        ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x0028))
#define GICC_APR0          ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x00D0))
#define GICC_NSAPR0        ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x00E0))
#define GICC_IIDR          ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x00FC))
#define GICC_DIR           ((unsigned int*)((unsigned char*)GIC_CPU_INTERFACE + 0x1000))

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned int EnableGrp0 : 1;
    unsigned int EnableGrp1 : 1;
    unsigned int RESERVED0 : 1;
    unsigned int FIQEn : 1;
    unsigned int CBPR : 1;
    unsigned int FIQBypDisGrp0 : 1;
    unsigned int IRQBypDisGrp0 : 1;
    unsigned int FIQBypDisGrp1 : 1;
    unsigned int IRQBypDisGrp1 : 1;
    unsigned int RESERVED1 : 23;
#else
    unsigned int RESERVED1 : 23;
    unsigned int IRQBypDisGrp1 : 1;
    unsigned int FIQBypDisGrp1 : 1;
    unsigned int IRQBypDisGrp0 : 1;
    unsigned int FIQBypDisGrp0 : 1;
    unsigned int CBPR : 1;
    unsigned int FIQEn : 1;
    unsigned int RESERVED0 : 1;
    unsigned int EnableGrp1 : 1;
    unsigned int EnableGrp0 : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE       
} gicc_ctlr_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int Priority : 8;
    unsigned long int RESERVED0 : 56;
#else
    unsigned long int RESERVED0 : 56;
    unsigned long int Priority : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE         
} gicc_pmr_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int CBPR_EL1S : 1;
    unsigned long int CBPR_EL1NS : 1;
    unsigned long int EOImode_EL3 : 1;
    unsigned long int EOImode_EL1S : 1;
    unsigned long int EOImode_EL1NS : 1;
    unsigned long int RM : 1;
    unsigned long int PMHE : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int PRIbits : 3;
    unsigned long int IDbits : 3;
    unsigned long int SEIS : 1;
    unsigned long int A3V : 1;
    unsigned long int RESERVED1 : 1;
    unsigned long int nDS : 1;
    unsigned long int RSS : 1;
    unsigned long int ExtRange : 1;
    unsigned long int RESERVED3 : 44;
#else
    unsigned long int RESERVED3 : 44;
    unsigned long int ExtRange : 1;
    unsigned long int RSS : 1;
    unsigned long int nDS : 1;
    unsigned long int RESERVED1 : 1;
    unsigned long int A3V : 1;
    unsigned long int SEIS : 1;
    unsigned long int IDbits : 3;
    unsigned long int PRIbits : 3;
    unsigned long int RESERVED0 : 1;
    unsigned long int PMHE : 1;
    unsigned long int RM : 1;
    unsigned long int EOImode_EL1NS : 1;
    unsigned long int EOImode_EL1S : 1;
    unsigned long int EOImode_EL3 : 1;
    unsigned long int CBPR_EL1NS : 1;
    unsigned long int CBPR_EL1S : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} gicc_ctlr_el3_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int CBPR : 1;
	unsigned long int EOImode : 1;
	unsigned long int RESERVED0 : 4;
	unsigned long int PMHE : 1;
	unsigned long int RESERVED1 : 1;
	unsigned long int PRIbits : 3;
	unsigned long int IDbits : 3;
    unsigned long int SEIS : 1;
    unsigned long int A3V : 1;
    unsigned long int RESERVED2 : 2;
	unsigned long int RSS : 1;
	unsigned long int ExtRange : 1;
	unsigned long int RESERVED3 : 44;
#else
	unsigned long int RESERVED3 : 44;
	unsigned long int ExtRange : 1;
	unsigned long int RSS : 1;
	unsigned long int RESERVED2 : 2;
	unsigned long int A3V : 1;
	unsigned long int SEIS : 1;
	unsigned long int IDbits : 3;
	unsigned long int PRIbits : 3;
	unsigned long int RESERVED1 : 1;
	unsigned long int PMHE : 1;
	unsigned long int RESERVED0 : 4;
	unsigned long int EOImode : 1;
	unsigned long int CBPR : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE 	
} gicc_ctlr_el1_t;

inline void gic_set_icc_ctlr_el3(gicc_ctlr_el3_t icc_ctlr_el3)
{
    // s3_6_c12_c12_4 is icc_ctlr_el3 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_6_c12_c12_4, %0" :: "r" (icc_ctlr_el3));
}

inline void gic_set_icc_ctlr_el1(gicc_ctlr_el1_t icc_ctlr_el1)
{
    // s3_0_c12_c12_4 is icc_ctlr_el1 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_0_c12_c12_4, %0" :: "r" (icc_ctlr_el1));
}

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int Priority : 8;
    unsigned long int RESERVED0 : 56;
#else
    unsigned long int RESERVED0 : 56;
    unsigned long int Priority : 8;
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} gicc_pmr_el1_t;

typedef enum
{
    GICC_PMR_EL1_PRIORITY_IN_STEPS_OF_16 = 16,
    GICC_PMR_EL1_PRIORITY_IN_STEPS_OF_8 = 32,
    GICC_PMR_EL1_PRIORITY_IN_STEPS_OF_4 = 64,
    GICC_PMR_EL1_PRIORITY_EVEN_VALUES_ONLY = 128,
    GICC_PMR_EL1_PRIORITY_ALL_VALUES = 256
} gicc_pmr_el1_priority_e;

inline void gic_set_icc_pmr_el1(gicc_pmr_el1_t icc_pmr_el1)
{
    // s3_0_c4_c6_0 is icc_pmr_el1 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_0_c4_c6_0, %0" :: "r" (icc_pmr_el1));
}

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int Binary_Point : 3;
    unsigned long int RESERVED0 : 61;
#else
    unsigned long int RESERVED0 : 61;
    unsigned long int Binary_Point : 3;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicc_bpr_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int INTID : 10;
    unsigned long int SGI_CPU_Interface : 3;
    unsigned long int Affinity : 11;
    unsigned long int RESERVED0 : 12;
#else
    unsigned long int RESERVED0 : 12;
    unsigned long int Affinity : 11;
    unsigned long int SGI_CPU_Interface : 3;
    unsigned long int INTID : 10;
#endif // ENDIANNESS == ENDIANNESS_LITTLE  
} gicc_iar_t, gicc_eoir_t;

#define GICC_IAR_SPURIOUS_INTID (0x3ff)	// 1023 means spurious interrupt

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
	unsigned int Implementer : 12;
	unsigned int Revision : 4;
	unsigned int ArchitectureVersion : 4;
	unsigned int ProductID : 12;
#else
	unsigned int ProductID : 12;
	unsigned int ArchitectureVersion : 4;
	unsigned int Revision : 4;
	unsigned int Implementer : 12;
#endif // ENDIANNESS == ENDIANNESS_LITTLE
} gicc_iidr_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int SRE : 1;
    unsigned long int DFB : 1;
    unsigned long int DIB : 1;
    unsigned long int Enable : 1;
    unsigned long int RESERVED0 : 60;
#else
    unsigned long int RESERVED0 : 60;
    unsigned long int Enable : 1;
    unsigned long int DIB : 1;
    unsigned long int DFB : 1;
    unsigned long int SRE : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} gicc_sre_el3_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int SRE : 1;
    unsigned long int DFB : 1;
    unsigned long int DIB : 1;
    unsigned long int RESERVED0 : 61;
#else
    unsigned long int RESERVED0 : 61;
    unsigned long int DIB : 1;
    unsigned long int DFB : 1;
    unsigned long int SRE : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} gicc_sre_el1_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int Enable : 1;
    unsigned long int RESERVED0 : 63;
#else
    unsigned long int RESERVED0 : 63;
    unsigned long int Enable : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE      
} gicc_igrpen0_el1_t;

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int Enable : 1;
    unsigned long int RESERVED0 : 63;
#else
    unsigned long int RESERVED0 : 63;
    unsigned long int Enable : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE      
} gicc_igrpen1_el1_t;

inline void gic_set_icc_sre_el3(gicc_sre_el3_t icc_sre_el3)
{
    // s3_6_c12_c12_5 is icc_sre_el3 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_6_c12_c12_5, %0" :: "r" (icc_sre_el3));
}

inline void gic_set_icc_sre_el1(gicc_sre_el1_t icc_sre_el1)
{
    // s3_0_c12_c12_5 is icc_sre_el1 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_0_c12_c12_5, %0" :: "r" (icc_sre_el1));
}

inline void gic_set_icc_igrpen0_el1(gicc_igrpen0_el1_t icc_igrpen0_el1)
{
    // s3_0_c12_c12_6 is icc_igrpen1_el1 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_0_c12_c12_6, %0" :: "r" (icc_igrpen0_el1));
}

inline void gic_set_icc_igrpen1_el1(gicc_igrpen1_el1_t icc_igrpen1_el1)
{
    // s3_0_c12_c12_7 is icc_igrpen1_el1 but the
    // compiler doesn't recognizes that built-in symbol
    asm ("msr s3_0_c12_c12_7, %0" :: "r" (icc_igrpen1_el1));
}

void gic_init();

void gic_timer_interrupt_init();
void gic_timer_interrupt_handle();

#endif // GIC_H