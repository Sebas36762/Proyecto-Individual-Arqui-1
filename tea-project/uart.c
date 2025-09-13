#include "uart.h"

#define UART0 0x10000000UL
static volatile uint32_t* const uart0 = (uint32_t*)UART0;

void uart_init(void) {
    // No se necesita configuración en QEMU 'virt'
}

void uart_putc(char c) {
    *uart0 = (uint32_t)c;
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void uart_puthex32(uint32_t v) {
    char hex[] = "0123456789ABCDEF";
    uart_puts("0x");
    for (int shift = 28; shift >= 0; shift -= 4) {
        uart_putc(hex[(v >> shift) & 0xF]);
    }
}

void uart_put_block(uint32_t left, uint32_t right) {
    // Mostrar en HEX
    uart_puthex32(left);
    uart_putc(' ');
    uart_puthex32(right);
    uart_puts("   -> \"");

    // Mostrar en ASCII legible
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
