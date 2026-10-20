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
        . = ALIGN(4096);
        _l1_table = .;
        . += 4096;
        . = ALIGN(8);
        _bss_end = .;
    }
    . = ALIGN(16);
    . += 0x4000;
    _stack_top = .;
    _kernel_end = .;
}
k1_platform_mem_reserve  = os_platform_mem_reserve;
k1_platform_mem_commit   = os_platform_mem_commit;
k1_platform_io_write     = os_platform_io_write;
k1_platform_io_is_tty    = os_platform_io_is_tty;
k1_platform_process_exit = os_platform_process_exit;
