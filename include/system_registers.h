#ifndef SYSTEM_REGISTERS_H
#define SYSTEM_REGISTERS_H

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE 
    unsigned long int NS : 1;
    unsigned long int IRQ : 1;
    unsigned long int IFQ : 1;
    unsigned long int EA : 1;
    unsigned long int RESERVED0 : 3;
    unsigned long int SMD : 1;
    unsigned long int HCE : 1;
    unsigned long int SIF : 1;
    unsigned long int RW : 1;
    unsigned long int ST : 1;
    unsigned long int TWI : 1;
    unsigned long int TWE : 1;
    unsigned long int TLOR : 1;
    unsigned long int TERR : 1;
    unsigned long int APK : 1;
    unsigned long int API : 1;
    unsigned long int EEL2 : 1;
    unsigned long int EASE : 1;
    unsigned long int NMEA : 1;
    unsigned long int FIEN : 1;
    unsigned long int RESERVED1 : 3;
    unsigned long int EnSCXT : 1;
    unsigned long int ATA : 1;
    unsigned long int FGTEn : 1;
    unsigned long int ECVEn : 1;
    unsigned long int TWEDEn : 1;
    unsigned long int TWEDEL : 4;
    unsigned long int RESERVED2 : 1;
    unsigned long int AMVOFFEN : 1;
    unsigned long int RESERVED3 : 27;
#else
    unsigned long int RESERVED3 : 27;
    unsigned long int AMVOFFEN : 1;
    unsigned long int RESERVED2 : 1;
    unsigned long int TWEDEL : 4;
    unsigned long int TWEDEn : 1;
    unsigned long int ECVEn : 1;
    unsigned long int FGTEn : 1;
    unsigned long int ATA : 1;
    unsigned long int EnSCXT : 1;
    unsigned long int RESERVED1 : 3;
    unsigned long int FIEN : 1;
    unsigned long int NMEA : 1;
    unsigned long int EASE : 1;
    unsigned long int EEL2 : 1;
    unsigned long int API : 1;
    unsigned long int APK : 1;
    unsigned long int TERR : 1;
    unsigned long int TLOR : 1;
    unsigned long int TWE : 1;
    unsigned long int TWI : 1;
    unsigned long int ST : 1;
    unsigned long int RW : 1;
    unsigned long int SIF : 1;
    unsigned long int HCE : 1;
    unsigned long int SMD : 1;
    unsigned long int RESERVED0 : 3;
    unsigned long int EA : 1;
    unsigned long int IFQ : 1;
    unsigned long int IRQ : 1;
    unsigned long int NS : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE 
} scr_el3_t;

// AArch64 state register version
typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int M : 4;
    unsigned long int M_AARCH64 : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int F : 1;
    unsigned long int I : 1;
    unsigned long int A : 1;
    unsigned long int D : 1;
    unsigned long int BTYPE : 2;
    unsigned long int SSBS : 1;
    unsigned long int RESERVED1 : 7;
    unsigned long int IL : 1;
    unsigned long int SS : 1;
    unsigned long int PAN : 1;
    unsigned long int UAO : 1;
    unsigned long int DIT : 1;
    unsigned long int TCO : 1;
    unsigned long int RESERVED2 : 2;
    unsigned long int V : 1;
    unsigned long int C : 1;
    unsigned long int Z : 1;
    unsigned long int N : 1;
    unsigned long int RESERVED3 : 32;
#else
    unsigned long int RESERVED3 : 32;
    unsigned long int N : 1;
    unsigned long int Z : 1;
    unsigned long int C : 1;
    unsigned long int V : 1;
    unsigned long int RESERVED2 : 2;
    unsigned long int TCO : 1;
    unsigned long int DIT : 1;
    unsigned long int UAO : 1;
    unsigned long int PAN : 1;
    unsigned long int SS : 1;
    unsigned long int IL : 1;
    unsigned long int RESERVED1 : 7;
    unsigned long int SSBS : 1;
    unsigned long int BTYPE : 2;
    unsigned long int D : 1;
    unsigned long int A : 1;
    unsigned long int I : 1;
    unsigned long int F : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int M_AARCH64 : 1;
    unsigned long int M : 4;    
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} spsr_el3_t;

typedef enum
{
    SPSR_EL3_M_EL0t = 0b0000,
    SPSR_EL3_M_EL1t = 0b0100,
    SPSR_EL3_M_EL1h = 0b0101,
    SPSR_EL3_M_EL2t = 0b1000,
    SPSR_EL3_M_EL2h = 0b1001,
    SPSR_EL3_M_EL3t = 0b1100,
    SPSR_EL3_M_EL3h = 0b1101
} spsr_el3_m_e;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int RESERVED0 : 8;
    unsigned long int EZ : 1;
    unsigned long int RESERVED1 : 1;
    unsigned long int TFP : 1;
    unsigned long int RESERVED2 : 9;
    unsigned long int TTA : 1;
    unsigned long int RESERVED3 : 9;
    unsigned long int TAM : 1;
    unsigned long int TCPAC : 1;
    unsigned long int RESERVED4 : 32;
#else
    unsigned long int RESERVED4 : 32;
    unsigned long int TCPAC : 1;
    unsigned long int TAM : 1;
    unsigned long int RESERVED3 : 9;
    unsigned long int TTA : 1;
    unsigned long int RESERVED2 : 9;
    unsigned long int TFP : 1;
    unsigned long int RESERVED1 : 1;
    unsigned long int EZ : 1;
    unsigned long int RESERVED0 : 8;

#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} cptr_el3_t;

inline void exception_level3_set_cptr(cptr_el3_t cptr_el3)
{
    asm ("msr cptr_el3, %0" :: "r" (cptr_el3));
}

inline void exception_level3_set_scr(scr_el3_t scr_el3)
{
    asm ("msr scr_el3, %0" :: "r" (scr_el3));
}

inline void exception_level3_set_spsr(spsr_el3_t spsr_el3)
{
    asm ("msr spsr_el3, %0" :: "r" (spsr_el3));
}

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int VM : 1;
    unsigned long int SWIO : 1;
    unsigned long int PTW : 1;
    unsigned long int FMO : 1;
    unsigned long int IMO : 1;
    unsigned long int AMO : 1;
    unsigned long int VF : 1;
    unsigned long int VI : 1;
    unsigned long int VSE : 1;
    unsigned long int FB : 1;
    unsigned long int BSU : 2;
    unsigned long int DC : 1;
    unsigned long int TWI : 1;
    unsigned long int TWE : 1;
    unsigned long int TID0 : 1;
    unsigned long int TID1 : 1;
    unsigned long int TID2 : 1;
    unsigned long int TID3 : 1;
    unsigned long int TSC : 1;
    unsigned long int TIDCP : 1;
    unsigned long int TACR : 1;
    unsigned long int TSW : 1;
    unsigned long int TPCP : 1;
    unsigned long int TPU : 1;
    unsigned long int TTLB : 1;
    unsigned long int TVM : 1;
    unsigned long int TGE : 1;
    unsigned long int TDZ : 1;
    unsigned long int HCD : 1;
    unsigned long int TRVM : 1;
    unsigned long int RW : 1;
    unsigned long int CD : 1;    
    unsigned long int ID : 1;
    unsigned long int E2H : 1;
    unsigned long int TLOR : 1;
    unsigned long int TERR : 1;
    unsigned long int TEA : 1;
    unsigned long int MIOCNCE : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int APK : 1;
    unsigned long int API : 1;
    unsigned long int NV : 1;
    unsigned long int NV1 : 1;
    unsigned long int AT : 1;
    unsigned long int NV2 : 1;
    unsigned long int FWB : 1;
    unsigned long int FIEN : 1;
    unsigned long int RESERVED1 : 1;
    unsigned long int TID4 : 1;
    unsigned long int TICAB : 1;
    unsigned long int AMVOFFEN : 1;
    unsigned long int TOCU : 1;
    unsigned long int EnSCXT : 1;
    unsigned long int TTLBIS : 1;
    unsigned long int TTLBOS : 1;
    unsigned long int ATA : 1;
    unsigned long int DCT : 1;
    unsigned long int TID5 : 1;
    unsigned long int TWEDEn : 1;
    unsigned long int TWEDEL : 4;
#else
    unsigned long int TWEDEL : 4;
    unsigned long int TWEDEn : 1;
    unsigned long int TID5 : 1;
    unsigned long int DCT : 1;
    unsigned long int ATA : 1;
    unsigned long int TTLBOS : 1;
    unsigned long int TTLBIS : 1;
    unsigned long int EnSCXT : 1;
    unsigned long int TOCU : 1;
    unsigned long int AMVOFFEN : 1;
    unsigned long int TICAB : 1;
    unsigned long int TID4 : 1;
    unsigned long int RESERVED1 : 1;   
    unsigned long int FIEN : 1;
    unsigned long int FWB : 1;
    unsigned long int NV2 : 1;
    unsigned long int AT : 1;
    unsigned long int NV1 : 1;
    unsigned long int NV : 1;
    unsigned long int API : 1;
    unsigned long int APK : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int MIOCNCE : 1;
    unsigned long int TEA : 1;
    unsigned long int TERR : 1;
    unsigned long int TLOR : 1;
    unsigned long int E2H : 1;
    unsigned long int ID : 1;
    unsigned long int CD : 1; 
    unsigned long int RW : 1;
    unsigned long int TRVM : 1;
    unsigned long int HCD : 1;
    unsigned long int TDZ : 1;
    unsigned long int TGE : 1;
    unsigned long int TVM : 1;
    unsigned long int TTLB : 1;
    unsigned long int TPU : 1;
    unsigned long int TPCP : 1;
    unsigned long int TSW : 1;
    unsigned long int TACR : 1;
    unsigned long int TIDCP : 1;
    unsigned long int TSC : 1;
    unsigned long int TID3 : 1;
    unsigned long int TID2 : 1;
    unsigned long int TID1 : 1;
    unsigned long int TID0 : 1;
    unsigned long int TWE : 1;
    unsigned long int TWI : 1;
    unsigned long int DC : 1;
    unsigned long int BSU : 2;
    unsigned long int FB : 1;
    unsigned long int VSE : 1;
    unsigned long int VI : 1;
    unsigned long int VF : 1;
    unsigned long int AMO : 1;
    unsigned long int IMO : 1;
    unsigned long int FMO : 1;
    unsigned long int PTW : 1;
    unsigned long int SWIO : 1;
    unsigned long int VM : 1;    
#endif // ENDIANNESS == ENDIANNESS_LITTLE 
} hcr_el2_t;

typedef enum
{
    SPSR_EL2_M_EL0t = 0b0000,
    SPSR_EL2_M_EL1t = 0b0100,
    SPSR_EL2_M_EL1h = 0b0101,
    SPSR_EL2_M_EL2t = 0b1000,
    SPSR_EL2_M_EL2h = 0b1001,
} spsr_el2_m_e;

// When FEAT_VHE is not implemented
typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int EL1PCTEN : 1;
    unsigned long int EL1PCEN : 1;
    unsigned long int EVNTEN : 1;
    unsigned long int EVNTDIR : 1;
    unsigned long int EVNTI : 4;
    unsigned long int RESERVED0 : 4;
    unsigned long int ECV : 1;
    unsigned long int EL1TVT : 1;
    unsigned long int EL1TVCT : 1;
    unsigned long int EL1NVPCT : 1;
    unsigned long int EL1NVVCT : 1;
    unsigned long int EVNTIS : 1;
    unsigned long int RESERVED1 : 46;
#else
    unsigned long int RESERVED1 : 46;
    unsigned long int EVNTIS : 1;
    unsigned long int EL1NVVCT : 1;
    unsigned long int EL1NVPCT : 1;
    unsigned long int EL1TVCT : 1;
    unsigned long int EL1TVT : 1;
    unsigned long int ECV : 1;
    unsigned long int RESERVED0 : 4;
    unsigned long int EVNTI : 4;
    unsigned long int EVNTDIR : 1;
    unsigned long int EVNTEN : 1;
    unsigned long int EL1PCEN : 1;
    unsigned long int EL1PCTEN : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE   
} cnthctl_el2_t;

inline void exception_level2_set_hcr(hcr_el2_t hcr_el2)
{
    asm ("msr hcr_el2, %0" :: "r" (hcr_el2));
}

inline void exception_level2_set_cnthctl_el2(cnthctl_el2_t cnthctl_el2)
{
    asm ("msr cnthctl_el2, %0" :: "r" (cnthctl_el2));
}

inline cnthctl_el2_t exception_level2_get_cnthctl_el2()
{
    cnthctl_el2_t cnthctl_el2;
    asm ("mrs %0, cnthctl_el2" : "=r" (cnthctl_el2));
    return cnthctl_el2;
}

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int M : 1;
    unsigned long int A : 1;
    unsigned long int C : 1;
    unsigned long int SA : 1;
    unsigned long int SA0 : 1;
    unsigned long int CP15BEN : 1;
    unsigned long int nAA : 1;
    unsigned long int ITD : 1;
    unsigned long int SED : 1;
    unsigned long int UMA : 1;
    unsigned long int EnRCTX : 1;
    unsigned long int EOS : 1;
    unsigned long int I : 1;
    unsigned long int EnDB : 1;
    unsigned long int DZE : 1;
    unsigned long int UCT : 1;
    unsigned long int nTWI : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int nTWE : 1;
    unsigned long int WXN : 1;
    unsigned long int TSCXT : 1;
    unsigned long int IESB : 1;
    unsigned long int EIS : 1;
    unsigned long int SPAN : 1;
    unsigned long int EOE : 1;
    unsigned long int EE : 1;
    unsigned long int UCI : 1;
    unsigned long int EnDA : 1;
    unsigned long int nTLSMD : 1;
    unsigned long int LSMAOE : 1;
    unsigned long int EnIB : 1;
    unsigned long int EnIA : 1;
    unsigned long int RESERVED1 : 3;
    unsigned long int BT0 : 1;
    unsigned long int BT1 : 1;
    unsigned long int ITFSB : 1;
    unsigned long int TCF0 : 2;
    unsigned long int TCF : 2;
    unsigned long int ATA0 : 1;
    unsigned long int ATA : 1;
    unsigned long int DSSBS : 1;
    unsigned long int TWEDEn : 1;
    unsigned long int TWEDEL : 4;
    unsigned long int TMT0 : 1;
    unsigned long int TMT : 1;
    unsigned long int TME0 : 1;
    unsigned long int TME : 1;
    unsigned long int RESERVED2 : 10;
#else
    unsigned long int RESERVED2 : 10;
    unsigned long int TME : 1;
    unsigned long int TME0 : 1;
    unsigned long int TMT : 1;
    unsigned long int TMT0 : 1;
    unsigned long int TWEDEL : 4;
    unsigned long int TWEDEn : 1;
    unsigned long int DSSBS : 1;
    unsigned long int ATA : 1;
    unsigned long int ATA0 : 1;
    unsigned long int TCF : 2;
    unsigned long int TCF0 : 2;
    unsigned long int ITFSB : 1;
    unsigned long int BT1 : 1;
    unsigned long int BT0 : 1;
    unsigned long int RESERVED1 : 3;
    unsigned long int EnIA : 1;
    unsigned long int EnIB : 1;
    unsigned long int LSMAOE : 1;
    unsigned long int nTLSMD : 1;
    unsigned long int EnDA : 1;
    unsigned long int UCI : 1;
    unsigned long int EE : 1;
    unsigned long int EOE : 1;
    unsigned long int SPAN : 1;
    unsigned long int EIS : 1;
    unsigned long int IESB : 1;
    unsigned long int TSCXT : 1;
    unsigned long int WXN : 1;
    unsigned long int nTWE : 1;
    unsigned long int RESERVED0 : 1;
    unsigned long int nTWI : 1;
    unsigned long int UCT : 1;
    unsigned long int DZE : 1;
    unsigned long int EnDB : 1;
    unsigned long int I : 1;
    unsigned long int EOS : 1;
    unsigned long int EnRCTX : 1;
    unsigned long int UMA : 1;
    unsigned long int SED : 1;
    unsigned long int ITD : 1;
    unsigned long int nAA : 1;
    unsigned long int CP15BEN : 1;
    unsigned long int SA0 : 1;
    unsigned long int SA : 1;
    unsigned long int C : 1;
    unsigned long int A : 1;
    unsigned long int M : 1;
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} sctlr_el1_t;

typedef struct
{
#if ENDIANNESS == ENDIANNESS_LITTLE
    unsigned long int RESERVED0 : 16;
    unsigned long int ZEN : 2;
    unsigned long int RESERVED1 : 2;
    unsigned long int FPEN : 2;
    unsigned long int RESERVED2 : 6;
    unsigned long int TTA : 1;
    unsigned long int RESERVED3 : 35;
#else
    unsigned long int RESERVED3 : 35;
    unsigned long int TTA : 1;
    unsigned long int RESERVED2 : 6;
    unsigned long int FPEN : 2;
    unsigned long int RESERVED1 : 2;
    unsigned long int ZEN : 2;
    unsigned long int RESERVED0 : 16;
#endif // ENDIANNESS == ENDIANNESS_LITTLE     
} cpacr_el1_t;

typedef enum
{
    CPACR_EL1_FPEN_EL0_EL1_TRAPPED = 0b00,
    CPACR_EL1_FPEN_EL0_TRAPPED_EL1_NOT_TRAPPED = 0b01,
    CPACR_EL1_FPEN_EL0_EL1_TRAPPED2 = 0b10,
    CPACR_EL1_FPEN_EL0_EL1_NOT_TRAPPED = 0b11
} cpacr_el1_fpen_e;

inline void exception_level1_set_sctlr(sctlr_el1_t sctlr_el1)
{
    asm ("msr sctlr_el1, %0" :: "r" (sctlr_el1));
}

inline void exception_level1_set_cpacr(cpacr_el1_t cpacr_el1)
{
    asm ("msr cpacr_el1, %0" :: "r" (cpacr_el1));
}

#endif // SYSTEM_REGISTERS_H