// tea.h
#ifndef TEA_H
#define TEA_H

#include <stdint.h>

void tea_encrypt_asm(uint32_t v[2], const uint32_t key[4]);
void tea_decrypt_asm(uint32_t v[2], const uint32_t key[4]);

#endif
