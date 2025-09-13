#!/bin/bash

# Build script for TEA project (C + Assembly + UART)
echo "Building TEA project..."

# Archivos fuente
C_SRC="main.c"
UART_SRC="uart.c"
STARTUP_SRC="startup.s"
TEA_ENCRYPT_SRC="tea_encrypt.s"
TEA_DECRYPT_SRC="tea_decrypt.s"
OUTPUT_ELF="tea.elf"

# Compilar main.c a objeto
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c "$C_SRC" -o main.o

if [ $? -ne 0 ]; then
    echo "C compilation failed"
    exit 1
fi

# Compilar uart.c a objeto
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c "$UART_SRC" -o uart.o

if [ $? -ne 0 ]; then
    echo "UART compilation failed"
    exit 1
fi

# Compilar startup assembly a objeto
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c "$STARTUP_SRC" -o startup.o
    
if [ $? -ne 0 ]; then
    echo "Startup assembly compilation failed"
    exit 1
fi

# Compilar TEA encrypt assembly a objeto
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c "$TEA_ENCRYPT_SRC" -o tea_encrypt.o

if [ $? -ne 0 ]; then
    echo "TEA encrypt assembly compilation failed"
    exit 1
fi

# Compilar TEA decrypt assembly a objeto
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    -c "$TEA_DECRYPT_SRC" -o tea_decrypt.o

if [ $? -ne 0 ]; then
    echo "TEA decrypt assembly compilation failed"
    exit 1
fi

# Linkear todos los objetos
riscv64-unknown-elf-gcc \
    -march=rv32im \
    -mabi=ilp32 \
    -nostdlib \
    -ffreestanding \
    -g3 \
    -gdwarf-4 \
    startup.o main.o uart.o tea_encrypt.o tea_decrypt.o \
    -T linker.ld \
    -o "$OUTPUT_ELF"

if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT_ELF created"
    echo "Object files: main.o, uart.o, startup.o, tea_encrypt.o, tea_decrypt.o"
else
    echo "Linking failed"
    exit 1
fi
