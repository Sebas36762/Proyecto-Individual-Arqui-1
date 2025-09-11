#include "uart.h"

#define UART0 0x10000000UL

static volatile uint32_t* const uart0 = (uint32_t*)UART0;

void uart_init(void) {
    // No se necesita configuración en QEMU 'virt'
}

void uart_putc(char c) {
    *uart0 = (uint32_t)c;  // Escribe el carácter en el registro UART
}

void uart_puts(const char *s) {
    while (*s) {
        uart_putc(*s++);
    }
}

void print_block(uint32_t v[2]) {
    char hex[] = "0123456789ABCDEF";
    for (int i = 0; i < 2; i++) {
        uart_puts("0x");
        for (int shift = 28; shift >= 0; shift -= 4) {
            uart_putc(hex[(v[i] >> shift) & 0xF]);
        }
        uart_putc(' ');
    }
    uart_putc('\n');
}
