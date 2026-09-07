# aarch64 bare-metal kernel: k1 (src/*.k1 directory module, root src/module.k1) + C drivers + boot.S
# usage: just / just run / just run-serial / just debug / just clean

out    := "out"
cflags := "--target=aarch64-none-elf -ffreestanding -mgeneral-regs-only -mstrict-align -std=gnu11 -Wall -Wextra -g -Isrc"
qemu   := "qemu-system-aarch64 -machine virt -cpu cortex-a72"

default: build

# compile the src/ k1 module (no-std, bare) + every C/asm source, link kernel.elf
build:
    #!/usr/bin/env bash
    set -euo pipefail
    mkdir -p {{out}}

    # --- k1: emit IR in bare no-std mode, then recompile it for our exact
    # target (ELF, strict alignment; FP/SIMD stays enabled - boot.S turns on the FPU)
    rm -f src/.k1-out/src.ll
    k1 --no-std --target arm64-bare --emit-llvm build src || true
    test -f src/.k1-out/src.ll || { echo "k1 compile failed"; exit 1; }
    clang --target=aarch64-none-elf -ffreestanding -mstrict-align \
        -Wno-override-module -g -c src/.k1-out/src.ll -o {{out}}/kernel_k1.o

    # --- C and assembly
    objs=({{out}}/kernel_k1.o)
    echo '[' > {{out}}/compile_commands.json
    for f in src/boot.S src/*.c src/drivers/*.c src/drivers/virtual/*.c src/lib/*.c; do
        o={{out}}/$(echo "${f#src/}" | tr '/' '_').o
        clang {{cflags}} -c "$f" -o "$o"
        printf '{"directory":"%s","file":"%s","command":"clang %s -c %s -o %s"},\n' \
            "$PWD" "$f" '{{cflags}}' "$f" "$o" >> {{out}}/compile_commands.json
        objs+=("$o")
    done
    sed -i '' '$ s/,$//' {{out}}/compile_commands.json && echo ']' >> {{out}}/compile_commands.json

    ld.lld -T src/linker.ld "${objs[@]}" -o {{out}}/kernel.elf
    echo "built {{out}}/kernel.elf"

# boot it: window + serial in this terminal
run: build
    {{qemu}} -device ramfb -serial stdio -kernel {{out}}/kernel.elf

# serial only, no window (Ctrl-A then X to quit)
run-serial: build
    {{qemu}} -nographic -kernel {{out}}/kernel.elf

# frozen at first instruction, gdb stub on :1234
debug: build
    {{qemu}} -device ramfb -serial stdio -kernel {{out}}/kernel.elf -s -S

clean:
    rm -rf {{out}} .k1-out src/.k1-out src/kernel/.k1-out
