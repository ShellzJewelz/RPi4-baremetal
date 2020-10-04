SECTIONS
{
    . = 0x80000;
    .text : { KEEP(*(.text.boot)) *(.text .text.* .gnu.linkonce.t*) }
    . = ALIGN(4096); /* align to page size */
    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r*) }
    . = ALIGN(4096); /* align to page size */
    PROVIDE(_data = .);
    .data : { *(.data .data.* .gnu.linkonce.d*) }
    . = ALIGN(4096); /* align to page size */
    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        __bss_end = .;
    }
    . = ALIGN(4096); /* align to page size */
    PROVIDE(_heap = .);
    . = 0x3BF80000;
    . = ALIGN(4096); /* align to page size */
    PROVIDE(_heap_end = .);
    PROVIDE(_stack_end = .);
    . = 0x3C000000;
    . = ALIGN(4096); /* align to page size */
    PROVIDE(_stack = .);
    
   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;