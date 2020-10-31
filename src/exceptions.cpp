#include "exceptions.h"
#include "hw_config.h"
#include "utils.h"

#if RPI_VERSION == RPI_VERSION_4
#include "gic.h"
#endif // RPI_VERSION != RPI_VERSION_4

typedef struct 
{
#if ENDIANNESS == ENDIANNESS_LITTLE
	unsigned long int ISS : 25;
	unsigned long int IL : 1;
	unsigned long int EC : 6;
	unsigned long int RESERVED0 : 32;
#else // ENDIANNESS == ENDIANNESS_BIG
	unsigned long int RESERVED0 : 32;
	unsigned long int EC : 6;
	unsigned long int IL : 1;
	unsigned long int ISS : 25;
#endif // ENDIANNESS == ENDIANNESS_LITTLE
} esr_elx_t;

typedef enum
{
	EXCEPTIONS_EC_UNKNOWN_REASON = 0b000000,
	EXCEPTIONS_EC_TRAPPED_WFI_WFE = 0b000001,

	// When AArch32 is supported at any Exception level
	EXCEPTIONS_EC_TRAPPED_MCR_MRC_COPROC_1111 = 0b000011, 
	EXCEPTIONS_EC_TRAPPED_MCRR_MRRC_COPROC_1111 = 0b000100,
	EXCEPTIONS_EC_TRAPPED_MCR_MRC_COPROC_1110 = 0b000101,
	EXCEPTIONS_EC_TRAPPED_LDC_STC = 0b000110,

	EXCEPTIONS_EC_TRAPPED_SVE_SIMD_FLOATING_POINT = 0b000111,

	// When AArch32 is supported at any Exception level
	EXCEPTIONS_EC_TRAPPED_VMRS_ACCESS = 0b001000,

	// When FEAT_PAuth is implemented
	EXCEPTIONS_EC_TRAPPED_POINTER_AUTHENTICATION = 0b001001,

	// When AArch32 is supported at any Exception level
	EXCEPTIONS_EC_TRAPPED_MRRC_COPROC_1110 = 0b001100,

	// When FEAT_BTI is implemented
	EXCEPTIONS_EC_BRANCH_TARGET_EXCEPTION = 0b001101,

	EXCEPTIONS_EC_ILLEGAL_EXECUTION_STATE = 0b001110,

	// When AArch32 is supported at any Exception level
	EXCEPTIONS_EC_SVC_INSTRUCTION_EXECUTION_AARCH32_STATE = 0b010001,
	EXCEPTIONS_EC_HVC_INSTRUCTION_EXECUTION_AARCH32_STATE = 0b010010,
	EXCEPTIONS_EC_SMC_INSTRUCTION_EXECUTION_AARCH32_STATE = 0b010011,
	
	// When AArch64 is supported at any Exception level
	EXCEPTIONS_EC_SVC_INSTRUCTION_EXECUTION_AARCH64_STATE = 0b010101,
	EXCEPTIONS_EC_HVC_INSTRUCTION_EXECUTION_AARCH64_STATE = 0b010110,
	EXCEPTIONS_EC_SMC_INSTRUCTION_EXECUTION_AARCH64_STATE = 0b010111,

	// When AArch64 is supported at any Exception level
	EXCEPTIONS_EC_TRAPPED_MSR_MRS_SYS_INSTRUCTION_AARCH64_STATE = 0b011000,

	// When FEAT_SVE is implemented
	EXCEPTIONS_EC_TRAPPED_SVE = 0b011001,

	// When FEAT_PAuth is implemented and FEAT_NV is implemented
	EXCEPTIONS_EC_TRAPPED_ERET_ERETAA_ERETAB = 0b011010,

	// When FEAT_FPAC is implemented
	EXCEPTIONS_EC_POINTER_AUTHENTICATION_FAILED = 0b011100,

	EXCEPTIONS_EC_INSTRUCTION_ABORT_LOWER_EXCEPTION_LEVEL = 0b100000,
	EXCEPTIONS_EC_INSTRUCTION_ABORT_CURRENT_EXCEPTION_LEVEL = 0b100001,
	EXCEPTIONS_EC_PC_ALIGNMENT_FAULT = 0b100010,
	EXCEPTIONS_EC_DATA_ABORT_LOWER_EXCEPTION_LEVEL = 0b100100,
	EXCEPTIONS_EC_DATA_ABORT_CURRENT_EXCEPTION_LEVEL = 0b100101,
	EXCEPTIONS_EC_SP_ALIGNMENT_FAULT = 0b100110,

	// When AArch32 is supported at any Exception level
	EXCEPTIONS_EC_TRAPPED_FLOATING_POINT_AARCH32_STATE = 0b101000,

	// When AArch64 is supported at any Exception level
	EXCEPTIONS_EC_TRAPPED_FLOATING_POINT_AARCH64_STATE = 0b101100,

	EXCEPTIONS_EC_SERROR_INTERRUPT = 0b101111,
	EXCEPTIONS_EC_BREAKPOINT_LOWER_EXCEPTION_LEVEL = 0b110000,
	EXCEPTIONS_EC_BREAKPOINT_CURRENT_EXCEPTION_LEVEL = 0b110001,
	EXCEPTIONS_EC_SOFTWARE_STEP_LOWER_EXCEPTION_LEVEL = 0b110010,
	EXCEPTIONS_EC_SOFTWARE_STEP_CURRENT_EXCEPTION_LEVEL = 0b110011,
	EXCEPTIONS_EC_WATCHPOINT_LOWER_EXCEPTION_LEVEL = 0b110100,
	EXCEPTIONS_EC_WATCHPOINT_CURRENT_EXCEPTION_LEVEL = 0b110101,

	// When AArch32 is supported at any Exception level
	EXCEPTIONS_EC_BKPT_INSTRUCTION_AARCH32_STATE = 0b111000,

	// When AArch64 is supported at any Exception level
	EXCEPTIONS_EC_BRK_INSTRUCTION_AARCH64_STATE = 0b111100

} exceptions_ec_e;

const char* exceptions_type_error_messages[] = 
{
	"SYNC_INVALID_EL1t",
	"IRQ_INVALID_EL1t",		
	"FIQ_INVALID_EL1t",		
	"ERROR_INVALID_EL1T",		

	"SYNC_INVALID_EL1h",		
	"IRQ_INVALID_EL1h",		
	"FIQ_INVALID_EL1h",		
	"ERROR_INVALID_EL1h",		

	"SYNC_INVALID_EL0_64",		
	"IRQ_INVALID_EL0_64",		
	"FIQ_INVALID_EL0_64",		
	"ERROR_INVALID_EL0_64",	

	"SYNC_INVALID_EL0_32",		
	"IRQ_INVALID_EL0_32",		
	"FIQ_INVALID_EL0_32",		
	"ERROR_INVALID_EL0_32",

	"SYNC_INVALID_EL2t",
	"IRQ_INVALID_EL2t",
	"FIQ_INVALID_EL2t",
	"ERROR_INVALID_EL2t",

	"SYNC_INVALID_EL2h",
	"IRQ_INVALID_EL2h",
	"FIQ_INVALID_EL2h",
	"ERROR_INVALID_EL2h",

	"SYNC_INVALID_EL1_64",
	"IRQ_INVALID_EL1_64",
	"FIQ_INVALID_EL1_64",
	"ERROR_INVALID_EL1_64",

	"SYNC_INVALID_EL1_32",
	"IRQ_INVALID_EL1_32",
	"FIQ_INVALID_EL1_32",
	"ERROR_INVALID_EL1_32",

	"SYNC_INVALID_EL3t",
	"IRQ_INVALID_EL3t",
	"FIQ_INVALID_EL3t",
	"ERROR_INVALID_EL3t",

	"SYNC_INVALID_EL3h",
	"IRQ_INVALID_EL3h",
	"FIQ_INVALID_EL3h",
	"ERROR_INVALID_EL3h",

	"SYNC_INVALID_EL2_64",
	"IRQ_INVALID_EL2_64",
	"FIQ_INVALID_EL2_64",
	"ERROR_INVALID_EL2_64",

	"SYNC_INVALID_EL2_32",
	"IRQ_INVALID_EL2_32",
	"FIQ_INVALID_EL2_32",
	"ERROR_INVALID_EL2_32",

	"UNKNOWN"
};

#define EXCEPTIONS_UNKNOWN (48)

const char* exceptions_class_error_messages[] =
{
	"UNKNOWN_REASON",
	"TRAPPED_WFI_WFE",
	"TRAPPED_MCR_MRC_COPROC_1111",
	"TRAPPED_MCRR_MRRC_COPROC_1111",
	"TRAPPED_MCR_MRC_COPROC_1110",
	"TRAPPED_LDC_STC",
	"TRAPPED_SVE_SIMD_FLOATING_POINT",
	"TRAPPED_VMRS_ACCESS",
	"TRAPPED_POINTER_AUTHENTICATION",
	"TRAPPED_MRRC_COPROC_1110",
	"BRANCH_TARGET_EXCEPTION",
	"ILLEGAL_EXECUTION_STATE",
	"SVC_INSTRUCTION_EXECUTION_AARCH32_STATE",
	"HVC_INSTRUCTION_EXECUTION_AARCH32_STATE",
	"SMC_INSTRUCTION_EXECUTION_AARCH32_STATE",
	"SVC_INSTRUCTION_EXECUTION_AARCH64_STATE",
	"HVC_INSTRUCTION_EXECUTION_AARCH64_STATE",
	"SMC_INSTRUCTION_EXECUTION_AARCH64_STATE",
	"TRAPPED_MSR_MRS_SYS_INSTRUCTION_AARCH64_STATE",
	"TRAPPED_SVE",
	"TRAPPED_ERET_ERETAA_ERETAB",
	"POINTER_AUTHENTICATION_FAILED",								
	"INSTRUCTION_ABORT_LOWER_EXCEPTION_LEVEL",
	"INSTRUCTION_ABORT_CURRENT_EXCEPTION_LEVEL",
	"PC_ALIGNMENT_FAULT",
	"DATA_ABORT_LOWER_EXCEPTION_LEVEL",
	"DATA_ABORT_CURRENT_EXCEPTION_LEVEL",
	"SP_ALIGNMENT_FAULT",
	"TRAPPED_FLOATING_POINT_AARCH32_STATE",
	"TRAPPED_FLOATING_POINT_AARCH64_STATE",
	"SERROR_INTERRUPT",
	"BREAKPOINT_LOWER_EXCEPTION_LEVEL",
	"BREAKPOINT_CURRENT_EXCEPTION_LEVEL",
	"SOFTWARE_STEP_LOWER_EXCEPTION_LEVEL",
	"SOFTWARE_STEP_CURRENT_EXCEPTION_LEVEL",
	"WATCHPOINT_LOWER_EXCEPTION_LEVEL",
	"WATCHPOINT_CURRENT_EXCEPTION_LEVEL",
	"BKPT_INSTRUCTION_AARCH32_STATE",
	"BRK_INSTRUCTION_AARCH64_STATE"
};

#define EXCEPTIONS_DATA_ABORT_ISS_DFSC_MASK (0b111111)
#define EXCEPTIONS_DATA_ABORT_ISS_WnR_MASK BIT(6)
#define EXCEPTIONS_DATA_ABORT_ISS_CM_MASK BIT(8)
#define EXCEPTIONS_DATA_ABORT_ISS_SET_MASK (BIT(11) | BIT(12))
#define EXCEPTIONS_DATA_ABORT_ISS_VNCR_MASK BIT(13)
#define EXCEPTIONS_DATA_ABORT_ISS_AR_MASK BIT(14)
#define EXCEPTIONS_DATA_ABORT_ISS_SF_MASK BIT(15) 
#define EXCEPTIONS_DATA_ABORT_ISS_SSE_MASK BIT(21)
#define EXCEPTIONS_DATA_ABORT_ISS_SAS_MASK (BIT(22) | BIT(23)) 
#define EXCEPTIONS_DATA_ABORT_ISS_ISV_MASK BIT(24)

typedef enum
{
	EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL0 = 0b000000,
	EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL1 = 0b000001,
	EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL2 = 0b000010,
	EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL3 = 0b000011,
	EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL0 = 0b000100,
	EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL1 = 0b000101,
	EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL2 = 0b000110,
	EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL3 = 0b000111,
	EXCEPTIONS_DATA_ABORT_DFSC_ACCESS_FLAG_FAULT_LEVEL1 = 0b001001,
	EXCEPTIONS_DATA_ABORT_DFSC_ACCESS_FLAG_FAULT_LEVEL2 = 0b001010,
	EXCEPTIONS_DATA_ABORT_DFSC_ACCESS_FLAG_FAULT_LEVEL3 = 0b001011,
	EXCEPTIONS_DATA_ABORT_DFSC_PREMISSION_FAULT_LEVEL1 = 0b001101,
	EXCEPTIONS_DATA_ABORT_DFSC_PREMISSION_FAULT_LEVEL2 = 0b001110,
	EXCEPTIONS_DATA_ABORT_DFSC_PREMISSION_FAULT_LEVEL3 = 0b001111,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_NOT_ON_TRANSLATION_TABLE_WALK = 0b010000,
	
	// When FEAT_MTE is implemented
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_TAG_CHECK_FAULT = 0b010001,

	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL0 = 0b010100,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL1 = 0b010101,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL2 = 0b010110,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL3 = 0b010111,

	//When FEAT_RAS is not implemented
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_NOT_ON_TRANSLATION_TABLE_WALK = 0b011000,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL0 = 0b011100,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL1 = 0b011101,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL2 = 0b011110,
	EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL3 = 0b011111,

	EXCEPTIONS_DATA_ABORT_DFSC_ALIGNMENT_FAULT = 0b100001,
	EXCEPTIONS_DATA_ABORT_DFSC_TLB_CONFLICT_ABORT = 0b110000,

	// When FEAT_HAFDBS is implemented
	EXCEPTIONS_DATA_ABORT_DFSC_UNSUPPORTED_ATOMIC_HARDWARE_UPDATE_FAULT = 0b110001,

	EXCEPTIONS_DATA_ABORT_DFSC_IMPLEMENTATION_DEFINED_FAULT_LOCKDOWN = 0b110100,
	EXCEPTIONS_DATA_ABORT_DFSC_IMPLEMENTATION_DEFINED_FAULT_UNSUPPORTED = 0b110101
} exceptions_data_abort_dfsc_e;

typedef enum
{
	EXCEPTIONS_DATA_ABORT_SET_RECOVERABLE_STATE = 0b00,
	EXCEPTIONS_DATA_ABORT_SET_UNCONTAINABLE = 0b01,
	EXCEPTIONS_DATA_ABORT_SET_RESTARTABLE_STATE = 0b10
} exceptions_data_abort_set_e;

typedef enum
{
	EXCEPTIONS_DATA_ABORT_SAS_BYTE = 0b00,
	EXCEPTIONS_DATA_ABORT_SAS_HALFWORD = 0b01,
	EXCEPTIONS_DATA_ABORT_SAS_WORD = 0b10,
	EXCEPTIONS_DATA_ABORT_SAS_DOUBLEWORD = 0b11
} exceptions_data_abort_sas_e;

static void exception_handle_data_abort(unsigned long int iss, char* data_abort_info, unsigned long int length);

#define EXCEPTIONS_SERROR_ISS_DFSC_MASK (0b11111)
#define EXCEPTIONS_SERROR_ISS_AET_MASK (BIT(10) | BIT(11) | BIT(12))
#define EXCEPTIONS_SERROR_ISS_IDS_MASK BIT(24)

typedef enum
{
	EXCEPTIONS_SERROR_DFSC_UNCATEGORIZED_ERROR = 0b000000,
	EXCEPTIONS_SERROR_DFSC_ASYNCHRONOUS_SERROR_INTERRUPT = 0b010001
} exceptions_serror_dfsc_e;

typedef enum
{
	EXCEPTIONS_SERROR_AET_UNCONTAINABLE = 0b000,
	EXCEPTIONS_SERROR_AET_UNRECOVERABLE = 0b001,
	EXCEPTIONS_SERROR_AET_RESTARTABLE_STATE = 0b010,
	EXCEPTIONS_SERROR_AET_RECOVERABLE_STATE = 0b011,
	EXCEPTIONS_SERROR_AET_CORRECTED = 0b110
} exceptions_serror_aet_e;

static void exception_handle_serror(unsigned long int iss, char* serror_info, unsigned long int length);

extern "C" void exceptions_invalid_entry_message(unsigned int type, esr_elx_t esr, unsigned long int elr)
{
	char iss_info[512] = { 0 };
	const char* exception_class = nullptr;

	if (type > (ARRAY_LENGTH(exceptions_type_error_messages) - 1))
	{
		type = EXCEPTIONS_UNKNOWN;
	}
	
	switch (esr.EC)
	{
	case EXCEPTIONS_EC_TRAPPED_WFI_WFE:
		exception_class = "Trapped WFI/WFE Instruction";
		break;
	case EXCEPTIONS_EC_TRAPPED_MCR_MRC_COPROC_1111:
		exception_class = "Trapped MCR/MRC Instruction (coproc = 1111)";
		break;
	case EXCEPTIONS_EC_TRAPPED_MCRR_MRRC_COPROC_1111:
		exception_class = "Trapped MCRR/MRRC Instruction (coproc = 1111)";
		break;
	case EXCEPTIONS_EC_TRAPPED_MCR_MRC_COPROC_1110:
		exception_class = "Trapped MCR/MRC Instruction (coproc = 1110)";
		break;
	case EXCEPTIONS_EC_TRAPPED_LDC_STC:
		exception_class = "Trapped LDC/STC Instruction";
		break;
	case EXCEPTIONS_EC_TRAPPED_SVE_SIMD_FLOATING_POINT:
		exception_class = "Trapped SVE/SIMD/Floating Point Instruction";
		break;
	case EXCEPTIONS_EC_TRAPPED_VMRS_ACCESS:
		exception_class = "Trapped VMRS Access";
		break;
	case EXCEPTIONS_EC_TRAPPED_POINTER_AUTHENTICATION:
		exception_class = "Trapped Pointer Authentication Instruction";
		break;
	case EXCEPTIONS_EC_TRAPPED_MRRC_COPROC_1110:
		exception_class = "Trapped MRRC (coproc = 1110)";
		break;
	case EXCEPTIONS_EC_BRANCH_TARGET_EXCEPTION:
		exception_class = "Branch Target Exception";
		break;
	case EXCEPTIONS_EC_ILLEGAL_EXECUTION_STATE:
		exception_class = "Illegal Execution State";
		break;
	case EXCEPTIONS_EC_SVC_INSTRUCTION_EXECUTION_AARCH32_STATE:
		exception_class = "SVC Instruction Execution Aarch32 State";
		break;
	case EXCEPTIONS_EC_HVC_INSTRUCTION_EXECUTION_AARCH32_STATE:
		exception_class = "HVC Instruction Execution Aarch32 State";
		break;
	case EXCEPTIONS_EC_SMC_INSTRUCTION_EXECUTION_AARCH32_STATE:
		exception_class = "SMC Instruction Execution Aarch32 State";
		break;
	case EXCEPTIONS_EC_SVC_INSTRUCTION_EXECUTION_AARCH64_STATE:
		exception_class = "SVC Instruction Execution Aarch64 State";
		break;
	case EXCEPTIONS_EC_HVC_INSTRUCTION_EXECUTION_AARCH64_STATE:
		exception_class = "HVC Instruction Execution Aarch64 State";
		break;
	case EXCEPTIONS_EC_SMC_INSTRUCTION_EXECUTION_AARCH64_STATE:
		exception_class = "SMC Instruction Execution Aarch64 State";
		break;
	case EXCEPTIONS_EC_TRAPPED_MSR_MRS_SYS_INSTRUCTION_AARCH64_STATE:
		exception_class = "Trapped MSR/MRS System Instruction Aarch64 State";
		break;
	case EXCEPTIONS_EC_TRAPPED_SVE:
		exception_class = "Trapped SVE Instruction";
		break;
	case EXCEPTIONS_EC_TRAPPED_ERET_ERETAA_ERETAB:
		exception_class = "Trapped ERET/ERETAA/ERETAB Instruction";
		break;		
	case EXCEPTIONS_EC_POINTER_AUTHENTICATION_FAILED:
		exception_class = "Pointer Authentication Failed";
		break;
	case EXCEPTIONS_EC_INSTRUCTION_ABORT_LOWER_EXCEPTION_LEVEL:
		exception_class = "Instruction Abort from Lower Exception Level";
		break;
	case EXCEPTIONS_EC_INSTRUCTION_ABORT_CURRENT_EXCEPTION_LEVEL:
		exception_class = "Instruction Abort from Current Exception Level";
		break;
	case EXCEPTIONS_EC_PC_ALIGNMENT_FAULT:
		exception_class = "PC Alignment Fault";
		break;
	case EXCEPTIONS_EC_DATA_ABORT_LOWER_EXCEPTION_LEVEL:
		exception_class = "Data Abort from Lower Exception Level";
		exception_handle_data_abort(esr.ISS, iss_info, sizeof(iss_info));
		break;
	case EXCEPTIONS_EC_DATA_ABORT_CURRENT_EXCEPTION_LEVEL:
		exception_class = "Data Abort from Current Exception Level";
		exception_handle_data_abort(esr.ISS, iss_info, sizeof(iss_info));
		break;
	case EXCEPTIONS_EC_SP_ALIGNMENT_FAULT:
		exception_class = "SP Alignment Fault";
		break;
	case EXCEPTIONS_EC_TRAPPED_FLOATING_POINT_AARCH32_STATE:
		exception_class = "Trapped Floating Point Instruction Aarch32 State";
		break;
	case EXCEPTIONS_EC_TRAPPED_FLOATING_POINT_AARCH64_STATE:
		exception_class = "Trapped Floating Point Instruction Aarch64 State";
		break;
	case EXCEPTIONS_EC_SERROR_INTERRUPT:
		exception_handle_serror(esr.ISS, iss_info, sizeof(iss_info));
		exception_class = "SError Interrupt";
		break;
	case EXCEPTIONS_EC_BREAKPOINT_LOWER_EXCEPTION_LEVEL:
		exception_class = "Breakpoint from Lower Exception Level";
		break;
	case EXCEPTIONS_EC_BREAKPOINT_CURRENT_EXCEPTION_LEVEL:
		exception_class = "Breakpoint from Current Exception Level";
		break;
	case EXCEPTIONS_EC_SOFTWARE_STEP_LOWER_EXCEPTION_LEVEL:
		exception_class = "Software Step from Lower Exception Level";
		break;
	case EXCEPTIONS_EC_SOFTWARE_STEP_CURRENT_EXCEPTION_LEVEL:
		exception_class = "Software Step from Current Exception Level";
		break;
	case EXCEPTIONS_EC_WATCHPOINT_LOWER_EXCEPTION_LEVEL:
		exception_class = "Watchpoint from Lower Exception Level";
		break;
	case EXCEPTIONS_EC_WATCHPOINT_CURRENT_EXCEPTION_LEVEL:
		exception_class = "Watchpoint from Current Exception Level";
		break;
	case EXCEPTIONS_EC_BKPT_INSTRUCTION_AARCH32_STATE:
		exception_class = "Breakpoint Instruction Aarch32 State";
		break;
	case EXCEPTIONS_EC_BRK_INSTRUCTION_AARCH64_STATE:
		exception_class = "Breakpoint Instruction Aarch64 State";
		break;
	case EXCEPTIONS_EC_UNKNOWN_REASON:
	default:
		exception_class = "Unknown";
		break;			
	}

	printf("\nException Type: %s\nException Class: %s\n", 
		exceptions_type_error_messages[type], exception_class);
	if (*iss_info)
	{
		printf("\n%s", iss_info);
	}
	printf("\nLast Instruction: 0x%X\n", elr);
}

extern "C" void exceptions_el1_irq_handler()
{
	printf("IRQ Occurred\n");
}

static void exception_handle_data_abort(unsigned long int iss, char* data_abort_info, unsigned long int length)
{	
	const char* data_abort_reason = nullptr;
	const char* data_abort_state = nullptr;
	const char* data_abort_access_size = nullptr;

	switch (iss & EXCEPTIONS_DATA_ABORT_ISS_DFSC_MASK)
	{
	case EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL0:
		data_abort_reason = "Address size fault, level 0";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL1:
		data_abort_reason = "Address size fault, level 1";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL2:
		data_abort_reason = "Address size fault, level 2";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ADDRESS_SIZE_FAULT_LEVEL3:
		data_abort_reason = "Address size fault, level 3";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL0:
		data_abort_reason = "Translation fault, level 0";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL1:
		data_abort_reason = "Translation fault, level 1";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL2:
		data_abort_reason = "Translation fault, level 2";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_TRANSLATION_FAULT_LEVEL3:
		data_abort_reason = "Translation fault, level 3";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ACCESS_FLAG_FAULT_LEVEL1:
		data_abort_reason = "Access flag fault, level 1";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ACCESS_FLAG_FAULT_LEVEL2:
		data_abort_reason = "Access flag fault, level 2";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ACCESS_FLAG_FAULT_LEVEL3:
		data_abort_reason = "Access flag fault, level 3";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_PREMISSION_FAULT_LEVEL1:
		data_abort_reason = "Permission fault, level 1";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_PREMISSION_FAULT_LEVEL2:
		data_abort_reason = "Permission fault, level 2";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_PREMISSION_FAULT_LEVEL3:
		data_abort_reason = "Permission fault, level 3";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_NOT_ON_TRANSLATION_TABLE_WALK:
	{
		data_abort_reason = "Synchronous External abort, not on translation table walk";
		switch ((iss & EXCEPTIONS_DATA_ABORT_ISS_SET_MASK) >> 11)
		{
			case EXCEPTIONS_DATA_ABORT_SET_RECOVERABLE_STATE:
				data_abort_state = "Recoverable state";
				break;
			case EXCEPTIONS_DATA_ABORT_SET_UNCONTAINABLE:
				data_abort_state = "Uncontainable";
				break;
			case EXCEPTIONS_DATA_ABORT_SET_RESTARTABLE_STATE:
				data_abort_state = "Restartable state";
				break;
		}
		break;
	}
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_TAG_CHECK_FAULT:
		data_abort_reason = "Synchronous Tag Check Fault";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL0:
		data_abort_reason = "Synchronous External abort, level 0";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL1:
		data_abort_reason = "Synchronous External abort, level 1";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL2:
		data_abort_reason = "Synchronous External abort, level 2";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_EXTERNAL_ABORT_LEVEL3:
		data_abort_reason = "Synchronous External abort, level 3";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_NOT_ON_TRANSLATION_TABLE_WALK:
		data_abort_reason = "Synchronous parity or ECC error on memory access, not on translation table walk";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL0:
		data_abort_reason = "Synchronous parity or ECC error on memory access, level 0";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL1:
		data_abort_reason = "Synchronous parity or ECC error on memory access, level 1";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL2:
		data_abort_reason = "Synchronous parity or ECC error on memory access, level 2";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_SYNCHRONOUS_PARITY_OR_ECC_ERROR_LEVEL3:
		data_abort_reason = "Synchronous parity or ECC error on memory access, level 3";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_ALIGNMENT_FAULT:
		data_abort_reason = "Alignment fault";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_TLB_CONFLICT_ABORT:
		data_abort_reason = "TLB conflict abort";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_UNSUPPORTED_ATOMIC_HARDWARE_UPDATE_FAULT:
		data_abort_reason = "Unsupported atomic hardware update fault";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_IMPLEMENTATION_DEFINED_FAULT_LOCKDOWN:
		data_abort_reason = "Implementatation defined fault (Lockdown)";
		break;
	case EXCEPTIONS_DATA_ABORT_DFSC_IMPLEMENTATION_DEFINED_FAULT_UNSUPPORTED:
		data_abort_reason = "Implementatation defined fault (Unsupported Exclusive or Atomic acces)";
		break;
	default:
		data_abort_reason = "Unknown";
		break;
	}

	switch ((iss & EXCEPTIONS_DATA_ABORT_ISS_SAS_MASK) >> 22)
	{
	case EXCEPTIONS_DATA_ABORT_SAS_BYTE:
		data_abort_access_size = "Byte";
		break;
	case EXCEPTIONS_DATA_ABORT_SAS_HALFWORD:
		data_abort_access_size = "Halfword";
		break;
	case EXCEPTIONS_DATA_ABORT_SAS_WORD:
		data_abort_access_size = "Word";
		break;
	case EXCEPTIONS_DATA_ABORT_SAS_DOUBLEWORD:
		data_abort_access_size = "Doubleword";
		break;
	default:
		data_abort_access_size = "Unknown";
		break;
	}

	sprintf(data_abort_info, 
		"Reason: %s\n"
		"State: %s\n"
		"Caused By: %s\n"
		"Cache Maintenance: %s\n"
		"VNCR: %s\n"
		"Acquire/Release: %s\n"
		"Register Width: %s\n"
		"Sign Extended: %s\n"
		"Access Size: %s\n", 
		data_abort_reason, 
		data_abort_state == nullptr ? "Unknown" : data_abort_state,
		iss & EXCEPTIONS_DATA_ABORT_ISS_WnR_MASK ? "Write" : "Read",
		iss & EXCEPTIONS_DATA_ABORT_ISS_VNCR_MASK ? "Yes" : "No",
		iss & EXCEPTIONS_DATA_ABORT_ISS_CM_MASK ? "Yes" : "No",
		(iss & EXCEPTIONS_DATA_ABORT_ISS_ISV_MASK && iss & EXCEPTIONS_DATA_ABORT_ISS_AR_MASK) ? "Yes" : "No",
		iss & EXCEPTIONS_DATA_ABORT_ISS_SF_MASK ? "32-bit" : "64-bit",
		(iss & EXCEPTIONS_DATA_ABORT_ISS_ISV_MASK && iss & EXCEPTIONS_DATA_ABORT_ISS_SSE_MASK) ? "Yes" : "No",
		data_abort_access_size
		);
}

static void exception_handle_serror(unsigned long int iss, char* serror_info, unsigned long int length)
{
	// Check for implementatino defined syncdrome bit
	if (!(iss & EXCEPTIONS_SERROR_ISS_IDS_MASK))
	{
		const char* serror_category = nullptr;
		const char* serror_state = nullptr;

		switch (iss & EXCEPTIONS_SERROR_ISS_DFSC_MASK)
		{
			case EXCEPTIONS_SERROR_DFSC_UNCATEGORIZED_ERROR:
				serror_category = "Uncategorized error";
				break;
			case EXCEPTIONS_SERROR_DFSC_ASYNCHRONOUS_SERROR_INTERRUPT:
			{
				serror_category = "Asynchronous SError interrupt";
				switch ((iss & EXCEPTIONS_SERROR_ISS_AET_MASK) >> 10)
				{
				case EXCEPTIONS_SERROR_AET_UNCONTAINABLE:
					serror_state = "Uncontainable";
					break;
				case EXCEPTIONS_SERROR_AET_UNRECOVERABLE:
					serror_state = "Unrecoverable";
					break;
				case EXCEPTIONS_SERROR_AET_RESTARTABLE_STATE:
					serror_state = "Restartable";
					break;
				case EXCEPTIONS_SERROR_AET_RECOVERABLE_STATE:				
					serror_state = "Recoverable";
					break;
				case EXCEPTIONS_SERROR_AET_CORRECTED:
					serror_state = "Corrected";
					break;
				}
				break;
			}
			default:
				serror_category = "Unknown";
				break;
		}

		printf("Serror iss: 0x%x", iss);

		sprintf(serror_info, 
			"Category: %s\n"
			"State: %s\n",
			serror_category,
			serror_state == nullptr ? "Unknown" : serror_state);
	}
}