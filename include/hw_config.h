#ifndef HW_CONFIG_H
#define HW_CONFIG_H

#define ENDIANNESS_LITTLE (0)
#define ENDIANNESS_BIG (1)
#define ENDIANNESS ENDIANNESS_LITTLE

#define DEBUG_HARDWARE_NO_HARDWARE (0)
#define DEBUG_HARDWARE_DISPLAY_HARDWARE (1)
#define DEBUG_HARDWARE_VIRTUAL_HARDWARE (2)
#define DEBUG_HARDWARE DEBUG_HARDWARE_DISPLAY_HARDWARE

#define RPI_VERSION_3 (3)
#define RPI_VERSION_4 (4)

#if RPI_VERSION == RPI_VERSION_3
#define MMIO_BASE                   ((void*)(0x3F000000))
#define GPU_CORE_FREQUENCY          (250000000)
#elif RPI_VERSION == RPI_VERSION_4
#define MMIO_BASE                   ((void*)(0xFE000000))
#define GPU_CORE_FREQUENCY          (2 * 250000000)
#else
#error "Unknown RPi Version"
#endif // RPI_VERSION == RPI_VERSION_3

#if DEBUG_HARDWARE == DEBUG_HARDWARE_DISPLAY_HARDWARE
#include "printf.h"
#include "frame_buffer.h"
#define putc frame_buffer_putc
#define puts frame_buffer_puts
#elif DEBUG_HARDWARE == DEBUG_HARDWARE_VIRTUAL_HARDWARE
#include "printf.h"
#include "uart.h"
#define putc uart_putc
#define puts uart_puts
#else
void inline dummy_printf(const char*, ...) {}
void dummy_sprintf(char*,const char *, ...) {}
void inline dummy_putc(const char) {}
void inline dummy_puts(const char*) {}
#define printf dummy_printf
#define sprintf dummy_sprintf
#define putc dummy_putc
#define puts dummy_puts
#endif // DEBUG_HARDWARE == DEBUG_HARDWARE_DISPLAY_HARDWARE

#define MEM_PAGE_SIZE               (4 * 1024)

#define VIDEOCORE_MAILBOX           ((void*)((unsigned char*)MMIO_BASE + 0xB880))
#define VIDEOCORE_MAILBOX_READ      ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x00))
#define VIDEOCORE_MAILBOX_POLL      ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x10))
#define VIDEOCORE_MAILBOX_SENDER    ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x14))
#define VIDEOCORE_MAILBOX_STATUS    ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x18))
#define VIDEOCORE_MAILBOX_CONFIG    ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x1C))
#define VIDEOCORE_MAILBOX_WRITE     ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x20))

#define SOC_MAILBOX                 ((void*)((unsigned char*)MMIO_BASE + 0xB8A0))
#define SOC_MAILBOX_READ            ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x00))
#define SOC_MAILBOX_POLL            ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x10))
#define SOC_MAILBOX_SENDER          ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x14))
#define SOC_MAILBOX_STATUS          ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x18))
#define SOC_MAILBOX_CONFIG          ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x1C))
#define SOC_MAILBOX_WRITE           ((unsigned int*)((unsigned char*)VIDEOCORE_MAILBOX + 0x20))

#define IRQ_BASIC_PENDING           ((void*)((unsigned char*)MMIO_BASE + 0x0000B200))
#define IRQ_PENDING_1	            ((void*)((unsigned char*)MMIO_BASE + 0x0000B204))
#define IRQ_PENDING_2		        ((void*)((unsigned char*)MMIO_BASE + 0x0000B208))
#define FIQ_CONTROL		            ((void*)((unsigned char*)MMIO_BASE + 0x0000B20C))
#define ENABLE_IRQS_1		        ((void*)((unsigned char*)MMIO_BASE + 0x0000B210))
#define ENABLE_IRQS_2		        ((void*)((unsigned char*)MMIO_BASE + 0x0000B214))
#define ENABLE_BASIC_IRQS	        ((void*)((unsigned char*)MMIO_BASE + 0x0000B218))
#define DISABLE_IRQS_1		        ((void*)((unsigned char*)MMIO_BASE + 0x0000B21C))
#define DISABLE_IRQS_2		        ((void*)((unsigned char*)MMIO_BASE + 0x0000B220))
#define DISABLE_BASIC_IRQS          ((void*)((unsigned char*)MMIO_BASE + 0x0000B224))

#define TIMER_BASE                  ((void*)((unsigned char*)MMIO_BASE + 0x3000))
#define TIMER_CONTROL_STATUS        ((unsigned int*)((unsigned char*)TIMER_BASE + 0x00))
#define TIMER_LOW                   ((unsigned int*)((unsigned char*)TIMER_BASE + 0x04))
#define TIMER_HIGH                  ((unsigned int*)((unsigned char*)TIMER_BASE + 0x08))
#define TIMER_COMPARE_REGISTER0     ((unsigned int*)((unsigned char*)TIMER_BASE + 0x0C))
#define TIMER_COMPARE_REGISTER1     ((unsigned int*)((unsigned char*)TIMER_BASE + 0x10))
#define TIMER_COMPARE_REGISTER2     ((unsigned int*)((unsigned char*)TIMER_BASE + 0x14))
#define TIMER_COMPARE_REGISTER3     ((unsigned int*)((unsigned char*)TIMER_BASE + 0x18))

#define GPIO_BASE                   ((void*)((unsigned char*)MMIO_BASE + 0x00200000))
#define GPIO_GPFSEL0                ((unsigned int*)((unsigned char*)GPIO_BASE + 0x00))
#define GPIO_GPFSEL1                ((unsigned int*)((unsigned char*)GPIO_BASE + 0x04)) 
#define GPIO_GPFSEL2                ((unsigned int*)((unsigned char*)GPIO_BASE + 0x08)) 
#define GPIO_GPFSEL3                ((unsigned int*)((unsigned char*)GPIO_BASE + 0x0C)) 
#define GPIO_GPFSEL4                ((unsigned int*)((unsigned char*)GPIO_BASE + 0x10)) 
#define GPIO_GPFSEL5                ((unsigned int*)((unsigned char*)GPIO_BASE + 0x14)) 
#define GPIO_GPSET0                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x1C)) 
#define GPIO_GPSET1                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x20))
#define GPIO_GPCLR0                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x28)) 
#define GPIO_GPCLR1                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x2C))
#define GPIO_GPLEV0                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x34)) 
#define GPIO_GPLEV1                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x38)) 
#define GPIO_GPEDS0                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x40)) 
#define GPIO_GPEDS1                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x44)) 
#define GPIO_GPHEN0                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x64)) 
#define GPIO_GPHEN1                 ((unsigned int*)((unsigned char*)GPIO_BASE + 0x68)) 
#define GPIO_GPPUD                  ((unsigned int*)((unsigned char*)GPIO_BASE + 0x94)) 
#define GPIO_GPPUDCLK0              ((unsigned int*)((unsigned char*)GPIO_BASE + 0x98)) 
#define GPIO_GPPUDCLK1              ((unsigned int*)((unsigned char*)GPIO_BASE + 0x9C))

#define UART0_BASE                  ((void*)(unsigned char*)GPIO_BASE + 0x1000)
#define UART0_DR                    ((unsigned int*)(unsigned char*)UART0_BASE + 0x00)
#define UART0_RSRECR                ((unsigned int*)(unsigned char*)UART0_BASE + 0x04)
#define UART0_FR                    ((unsigned int*)(unsigned char*)UART0_BASE + 0x18)
#define UART0_ILPR                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x20)
#define UART0_IBRD                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x24)
#define UART0_FBRD                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x28)
#define UART0_LCRH                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x2C)
#define UART0_CR                    ((unsigned int*)(unsigned char*)UART0_BASE + 0x30)
#define UART0_IFLS                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x34)
#define UART0_IMSC                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x38)
#define UART0_RIS                   ((unsigned int*)(unsigned char*)UART0_BASE + 0x3C)
#define UART0_MIS                   ((unsigned int*)(unsigned char*)UART0_BASE + 0x40)
#define UART0_ICR                   ((unsigned int*)(unsigned char*)UART0_BASE + 0x44)
#define UART0_DMACR                 ((unsigned int*)(unsigned char*)UART0_BASE + 0x48)
#define UART0_ITCR                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x80)
#define UART0_ITIP                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x84)
#define UART0_ITOP                  ((unsigned int*)(unsigned char*)UART0_BASE + 0x88)
#define UART0_TDR                   ((unsigned int*)(unsigned char*)UART0_BASE + 0x8C)

#define UART1_BASE                  ((unsigned int*)((unsigned char*)MMIO_BASE + 0x00215000))
#define UART1_IRQ                   ((unsigned int*)((unsigned char*)MMIO_BASE + 0x00))
#define UART1_ENABLE                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x04))
#define UART1_MU_IO                 ((unsigned int*)((unsigned char*)MMIO_BASE + 0x40))
#define UART1_MU_IER                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x44))
#define UART1_MU_IIR                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x48))
#define UART1_MU_LCR                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x4C))
#define UART1_MU_MCR                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x50))
#define UART1_MU_LSR                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x54))
#define UART1_MU_MSR                ((unsigned int*)((unsigned char*)MMIO_BASE + 0x58))
#define UART1_MU_SCRATCH            ((unsigned int*)((unsigned char*)MMIO_BASE + 0x5C))
#define UART1_MU_CNTL               ((unsigned int*)((unsigned char*)MMIO_BASE + 0x60))
#define UART1_MU_STAT               ((unsigned int*)((unsigned char*)MMIO_BASE + 0x64))
#define UART1_MU_BAUD               ((unsigned int*)((unsigned char*)MMIO_BASE + 0x68))

#endif // HW_CONFIG_H