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


# --- hello.k1 ----------------------------------------------------------------
# k1 only emits host objects (Mach-O arm64 here), which ld.lld cannot link into
# an ELF kernel. Take its LLVM IR instead and re-codegen it for aarch64-none-elf.
set(K1_SRC ${CMAKE_SOURCE_DIR}/hello.k1)
set(K1_LL  ${CMAKE_SOURCE_DIR}/.k1-out/hello.ll)
set(K1_OBJ ${CMAKE_CURRENT_BINARY_DIR}/hello_k1.o)

add_custom_command(
    OUTPUT ${K1_LL}
    COMMAND k1 --emit-llvm build ${K1_SRC}
    DEPENDS ${K1_SRC}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "k1 build hello.k1 -> hello.ll"
    VERBATIM
)

add_custom_command(
    OUTPUT ${K1_OBJ}
    COMMAND ${CMAKE_C_COMPILER} --target=aarch64-none-elf -ffreestanding
            -mgeneral-regs-only -Wno-override-module -c ${K1_LL} -o ${K1_OBJ}
    DEPENDS ${K1_LL}
    COMMENT "hello.ll -> aarch64-none-elf object"
    VERBATIM
)

set_source_files_properties(${K1_OBJ} PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)

add_executable(kernel.elf
    src/boot.S
    src/kernel.c
    src/power.c
    src/console.c
    src/drivers/pl011.c
    src/drivers/virtual/ramfb.c
    src/drivers/virtual/fwcfg.c
    src/lib/string.c
        src/lib/printf.c
        src/lib/printf.h
        src/lib/mem.c
        src/lib/mem.h
        src/gfx.c
        src/gfx.h
        ${K1_OBJ}
)

# lets any file include project headers rooted at src/, e.g. "lib/string.h"
target_include_directories(kernel.elf PRIVATE src)

target_compile_options(kernel.elf PRIVATE
    -ffreestanding
    -mgeneral-regs-only
    -mstrict-align # MMU off = Device memory = unaligned accesses fault
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
