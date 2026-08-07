ENTRY(_start)

SECTIONS
{
    . = 0x40000000;
    .text : {
        *(.text.boot)
        *(.text .text.*)
    }
    .rodata : { *(.rodata .rodata.*) }
    .data   : { *(.data .data.*) }
    .bss : {
        . = ALIGN(8);
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(8);
        _bss_end = .;
    }
    . = ALIGN(16);
    . += 0x4000;
    _stack_top = .;
    _kernel_end = .;
}