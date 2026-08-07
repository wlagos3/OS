cmake_minimum_required(VERSION 3.20)

# Bare-metal AArch64 target: no host OS, no libc, no test-binary linking.
# These must be set before project() so CMake's compiler checks use them.
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)
set(CMAKE_C_COMPILER clang)
set(CMAKE_ASM_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET aarch64-none-elf)
set(CMAKE_ASM_COMPILER_TARGET aarch64-none-elf)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(os C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON) # gnu11: asm(), typeof, attributes

find_program(LLD ld.lld REQUIRED PATHS /opt/homebrew/opt/lld/bin)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/src/linker.ld)

add_executable(kernel.elf
    src/boot.S
        src/kernel.c
)

target_compile_options(kernel.elf PRIVATE
    -ffreestanding
    -mgeneral-regs-only
    -Wall
    -Wextra
    -g
)

target_link_options(kernel.elf PRIVATE
    --ld-path=${LLD}
    -nostdlib
    -Wl,-T,${LINKER_SCRIPT}
)

# relink when the linker script changes
set_target_properties(kernel.elf PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

# `cmake --build . --target run` (or the CLion "run" target) boots the kernel.
# Ctrl-A then X exits QEMU in -nographic mode.
add_custom_target(run
    COMMAND qemu-system-aarch64
        -machine virt
        -cpu cortex-a72
        -nographic
        -kernel $<TARGET_FILE:kernel.elf>
    DEPENDS kernel.elf
    USES_TERMINAL
)

# same, but frozen at the first instruction waiting for a debugger on :1234
add_custom_target(debug
    COMMAND qemu-system-aarch64
        -machine virt
        -cpu cortex-a72
        -nographic
        -kernel $<TARGET_FILE:kernel.elf>
        -s -S
    DEPENDS kernel.elf
    USES_TERMINAL
)
