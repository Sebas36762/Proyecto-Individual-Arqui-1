    .section .text
    .globl tea_decrypt_asm
tea_decrypt_asm:
    # Entradas:
    # a0 = puntero a v[2]
    # a1 = puntero a key[4]

    # Cargar v0 y v1
    lw t0, 0(a0)      # t0 = v[0]
    lw t1, 4(a0)      # t1 = v[1]

    # Cargar la clave en s0-s3
    lw s0, 0(a1)      # key[0]
    lw s1, 4(a1)      # key[1]
    lw s2, 8(a1)      # key[2]
    lw s3, 12(a1)     # key[3]

    li t3, 32         # contador de rondas
    li t4, 0x9e3779b9 # DELTA
    li t2, 32
    mul t2, t2, t4    # sum = DELTA * 32

decrypt_loop:
    # v1 -= ((v0 << 4) + key[2]) ^ (v0 + sum) ^ ((v0 >> 5) + key[3])
    sll t5, t0, 4
    add t5, t5, s2
    add t6, t0, t2
    xor t5, t5, t6
    srl t6, t0, 5
    add t6, t6, s3
    xor t5, t5, t6
    sub t1, t1, t5

    # v0 -= ((v1 << 4) + key[0]) ^ (v1 + sum) ^ ((v1 >> 5) + key[1])
    sll t5, t1, 4
    add t5, t5, s0
    add t6, t1, t2
    xor t5, t5, t6
    srl t6, t1, 5
    add t6, t6, s1
    xor t5, t5, t6
    sub t0, t0, t5

    sub t2, t2, t4    # sum -= DELTA
    addi t3, t3, -1
    bnez t3, decrypt_loop

    # Guardar resultados
    sw t0, 0(a0)
    sw t1, 4(a0)

    ret
