#include <stdio.h>
#include <stdint.h>
#include "tea.h"

int main() {
    uint32_t v[2] = {0x484F4C41, 0x31323334}; // "HOLA1234"
    uint32_t key[4] = {0x12345678, 0x9ABCDEF0, 0xFEDCBA98, 0x76543210};

    printf("Texto original: %08X %08X\n", v[0], v[1]);

    tea_encrypt_asm(v, key);
    printf("Cifrado: %08X %08X\n", v[0], v[1]);

    tea_decrypt_asm(v, key);
    printf("Descifrado: %08X %08X\n", v[0], v[1]);

    return 0;
}
