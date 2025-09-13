#include "uart.h"

#define UART0 0x10000000UL  
// Dirección base del dispositivo UART0 en QEMU virt (MMIO)
static volatile uint32_t* const uart0 = (uint32_t*)UART0;

void uart_init(void) {

}

void uart_putc(char c) {
    // Escribir un solo carácter en el UART
    *uart0 = (uint32_t)c;
}

void uart_puts(const char *s) {
    // Imprimir una cadena terminada en '\0'
    while (*s) {
        uart_putc(*s++); // enviar carácter por carácter
    }
}

void uart_puthex32(uint32_t v) {
    // Imprimir un entero de 32 bits en formato hexadecimal
    char hex[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int shift = 28; shift >= 0; shift -= 4) {
        uart_putc(hex[(v >> shift) & 0xF]);
    }
}

void uart_put_block(uint32_t left, uint32_t right) {
    // Mostrar un bloque de 64 bits (dos palabras de 32 bits)

    // 1. Imprimir en hexadecimal
    uart_puthex32(left);
    uart_putc(' ');
    uart_puthex32(right);
    uart_puts("   -> \"");

    // 2. Mostrar en ASCII legible
    //    Si un byte está fuera del rango imprimible (32..126),
    //    se sustituye por '.'
    for (int i = 0; i < 4; i++) {
        char c = (left >> (24 - i*8)) & 0xFF;
        uart_putc((c >= 32 && c <= 126) ? c : '.');
    }
    for (int i = 0; i < 4; i++) {
        char c = (right >> (24 - i*8)) & 0xFF;
        uart_putc((c >= 32 && c <= 126) ? c : '.');
    }

    uart_puts("\"\n");
}
