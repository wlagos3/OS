.section ".text.boot"
.global _start

_start:
// park all the cores except core 0
    mrs x1, mpidr_el1
    and x1, x1, #0xFF
    cbz x1, proceed
core_hang:
    wfe
    b core_hang
proceed:
// setup stack pointer, 16kb will be reserved during linking at _stack_top
    ldr x1, =_stack_top
    mov sp, x1

// zero out the global init to 0 space
    ldr x1, =_bss_start
    ldr x2, =_bss_end
1:  cmp x1, x2
    b.hs 2f
    str xzr, [x1], #8
    b 1b
2:
    bl kernel_main
    b core_hang


