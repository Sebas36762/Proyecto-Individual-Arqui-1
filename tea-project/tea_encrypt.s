    .section .text
    .globl tea_encrypt_asm
tea_encrypt_asm:
    # a0 = puntero a bloque v[2]
    # a1 = puntero a clave key[4]

    addi sp, sp, -32       # reservar espacio para guardar s0-s7
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)

    # Cargar valores del bloque
    lw s0, 0(a0)   # v0
    lw s1, 4(a0)   # v1

    # Cargar clave en registros
    lw s2, 0(a1)   # key[0]
    lw s3, 4(a1)   # key[1]
    lw s4, 8(a1)   # key[2]
    lw s5, 12(a1)  # key[3]

    li s6, 0x9e3779b9  # DELTA
    li s7, 0           # sum
    li t0, 32          # contador de rondas

encrypt_loop:
    add s7, s7, s6

    # v0 += ((v1<<4)+key[0]) ^ (v1+sum) ^ ((v1>>5)+key[1])
    slli t1, s1, 4
    add t1, t1, s2
    add t2, s1, s7
    srai t3, s1, 5
    add t3, t3, s3
    xor t1, t1, t2
    xor t1, t1, t3
    add s0, s0, t1

    # v1 += ((v0<<4)+key[2]) ^ (v0+sum) ^ ((v0>>5)+key[3])
    slli t1, s0, 4
    add t1, t1, s4
    add t2, s0, s7
    srai t3, s0, 5
    add t3, t3, s5
    xor t1, t1, t2
    xor t1, t1, t3
    add s1, s1, t1

    addi t0, t0, -1
    bnez t0, encrypt_loop

    # Guardar resultados
    sw s0, 0(a0)
    sw s1, 4(a0)

    # Restaurar registros
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    addi sp, sp, 32
    ret
