SECTIONS
{
    PROVIDE(_stack_end = .);
    . = 0x80000;
    PROVIDE(_stack = .);
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
    .init.array : { 
        PROVIDE(__init_array_start = .);
        KEEP(*(SORT(.init_array.*)))
        KEEP(*(.init_array*))
        PROVIDE(__init_array_end = .); 
    }
    . = ALIGN(4096); /* align to page size */
    PROVIDE(_heap = .);
    . = 0x3C000000;
    PROVIDE(_heap_end = .);
   /DISCARD/ : { *(.comment) *(.gnu*) *(.note*) *(.eh_frame*) }
}
__bss_size = (__bss_end - __bss_start)>>3;