#include <stdint.h>
#include "uart.h"

// Declaraciones de funciones ASM
void tea_encrypt_asm(uint32_t v[2], const uint32_t key[4]);
void tea_decrypt_asm(uint32_t v[2], const uint32_t key[4]);

// Bloques globales para inspección
#define MAX_BLOCKS 16
uint32_t blocks[MAX_BLOCKS*2]; // cada bloque = 2 palabras (64 bits)

/**
 * Función que:
 *  - Convierte un mensaje ASCII en bloques de 64 bits.
 *  - Aplica padding si la longitud no es múltiplo de 8 (se rellenan bytes con ceros).
 *  - Cifra con TEA en ASM.
 *  - Imprime los bloques cifrados y descifrados.
 *  - Reconstruye y muestra el mensaje final en ASCII.
 */

void encrypt_decrypt_print(uint8_t *msg, int len, uint32_t key[4], const char* label) {
    int num_blocks = (len + 7) / 8; // padding

    // Convertir mensaje a bloques de 64 bits con padding
    for (int i = 0; i < num_blocks; i++) {
        blocks[i*2 + 0] = 0;
        blocks[i*2 + 1] = 0;
        for (int j = 0; j < 8; j++) {
            if (i*8 + j < len) {
                if (j < 4)
                    blocks[i*2 + 0] |= msg[i*8 + j] << (8*(3-j));
                else
                    blocks[i*2 + 1] |= msg[i*8 + j] << (8*(7-j));
            }
        }
    }

    uart_puts("\n==============================\n");
    uart_puts(label);
    uart_puts(" - Bloques originales:\n");
    for (int i = 0; i < num_blocks; i++)
        uart_put_block(blocks[i*2], blocks[i*2 + 1]);

    // Cifrar
    for (int i = 0; i < num_blocks; i++)
        tea_encrypt_asm(&blocks[i*2], key);

    uart_puts(label);
    uart_puts(" - Bloques cifrados:\n");
    for (int i = 0; i < num_blocks; i++)
        uart_put_block(blocks[i*2], blocks[i*2 + 1]);

    // Descifrar
    for (int i = 0; i < num_blocks; i++)
        tea_decrypt_asm(&blocks[i*2], key);

    uart_puts(label);
    uart_puts(" - Bloques descifrados:\n");
    for (int i = 0; i < num_blocks; i++)
        uart_put_block(blocks[i*2], blocks[i*2 + 1]);

    // Mostrar mensaje final en ASCII
    uart_puts(label);
    uart_puts(" - Mensaje final:\n\"");
    for (int i = 0; i < num_blocks; i++) {
        for (int j = 0; j < 8 && (i*8 + j) < len; j++) {
            uint32_t word = blocks[i*2 + j/4];
            char c = (word >> (8*(3-(j%4)))) & 0xFF;
            uart_putc(c);
        }
    }
    uart_puts("\"\n");
}

int main() {
    uart_init();

    // Prueba 1 - Bloque único
    uint8_t message1[] = "HOLA1234";
    uint32_t key1[4]  = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};
    encrypt_decrypt_print(message1, sizeof(message1)-1, key1, "Prueba 1");

    // Prueba 2 - Múltiples bloques
    uint8_t message2[] = "Mensaje de prueba para TEA";
    uint32_t key2[4]  = {0x0F1E2D3C, 0x4B5A6978, 0x8796A5B4, 0xC3D2E1F0};
    encrypt_decrypt_print(message2, sizeof(message2)-1, key2, "Prueba 2");

    uart_puts("\n===== FIN DE LAS PRUEBAS =====\n");

    while(1); // Detenerse para inspección en GDB
    return 0;
}
