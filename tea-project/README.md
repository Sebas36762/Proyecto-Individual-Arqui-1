# Implementación del algoritmo TEA en RISC-V con UART, QEMU y GDB  

Este proyecto implementa el **Tiny Encryption Algorithm (TEA)** en un entorno **bare-metal RISC-V de 32 bits**, combinando código en C y ensamblador. La salida de resultados se muestra a través del **UART emulado por QEMU** y se brinda soporte de depuración con **GDB**.  

---

## 1. Estructura del proyecto  
```
.
├── Dockerfile
├── run.sh
├── tea-project/
│   ├── build.sh           # Script de compilación
│   ├── run-qemu.sh        # Script para ejecutar en QEMU con GDB
│   ├── linker.ld          # Script de linkeo
│   ├── startup.s          # Inicio y configuración mínima del stack
│   ├── main.c             # Lógica principal en C
│   ├── uart.c             # Implementación de UART
│   ├── uart.h             # Encabezado de UART
│   ├── tea_encrypt.s      # Cifrado TEA en ensamblador
│   ├── tea_decrypt.s      # Descifrado TEA en ensamblador
│   └── README.md          # Documentación
```

- **`main.c`**: lógica de pruebas, bloques, padding, llamadas a TEA.  
- **`uart.c / uart.h`**: comunicación con UART.  
- **`tea_encrypt.s` y `tea_decrypt.s`**: implementación del algoritmo en ensamblador.  
- **`startup.s`**: inicialización mínima antes de entrar a `main`.  
- **`build.sh`**: compila el proyecto.  
- **`run-qemu.sh`**: ejecuta QEMU en modo nographic con GDB.  

---

## 2. Descripción de la arquitectura del software  

El software se organiza en dos capas:  

- **C (alto nivel):**  
  - Manejo de bloques, padding, pruebas y salida por UART.  
  - Llamadas a las funciones de cifrado/descifrado en ensamblador.  

- **Ensamblador (bajo nivel):**  
  - Implementación eficiente del TEA (cifrado/descifrado).  
  - Rutinas independientes de la lógica de pruebas.  

**Interfaz utilizada:**  
- `tea_encrypt_asm(uint32_t v[2], const uint32_t key[4])`  
- `tea_decrypt_asm(uint32_t v[2], const uint32_t key[4])`  

**Justificación de diseño:**  
- C controla pruebas y mensajes.  
- Ensamblador se encarga de cálculos críticos (TEA).  
- UART en C para mayor simplicidad en depuración.  

---

## 3. Explicación de las funcionalidades implementadas  

- **Cifrado TEA** de 64 bits con clave de 128 bits (32 rondas).  
- **Descifrado TEA** que recupera el mensaje original.  
- **Padding automático** al dividir en bloques de 8 bytes.  
- **Impresión UART** de:  
  - Bloques originales  
  - Bloques cifrados  
  - Bloques descifrados  
  - Mensaje final en ASCII  
- **Pruebas incluidas:**  
  - **Prueba 1:** mensaje de 1 bloque (`"HOLA1234"`).  
  - **Prueba 2:** mensaje de múltiples bloques (`"Mensaje de prueba para TEA"`).  

---

## 4. Evidencias de ejecución en QEMU y GDB  

### Salida UART en QEMU  
```
==============================
Prueba 1 - Bloques originales:
0x484F4C41 0x31323334
Prueba 1 - Bloques cifrados:
0x51A905F6 0xCA06B672
Prueba 1 - Bloques descifrados:
0x484F4C41 0x31323334
Prueba 1 - Mensaje final:
HOLA1234
==============================
Prueba 2 - Bloques originales:
0x4D656E73 0x616A6520
0x64652070 0x72756562
0x61207061 0x72612054
0x45410000 0x00000000
Prueba 2 - Bloques cifrados:
0xC3D80575 0xE2F32FAE
0x99DB75B2 0x33060D9A
0xD85D7207 0x90D13787
0x06FB60BB 0x5C1F21D5
Prueba 2 - Bloques descifrados:
0x4D656E73 0x616A6520
0x64652070 0x72756562
0x61207061 0x72612054
0x45410000 0x00000000
Prueba 2 - Mensaje final:
Mensaje de prueba para TEA
```

### Sesión de GDB  
```
$ gdb-multiarch tea.elf
(gdb) target remote :1234
Remote debugging using :1234
0x00001000 in ?? ()
(gdb) break main
Breakpoint 1 at 0x8000047c: file main.c, line 66.
(gdb) continue
Continuing.

Breakpoint 1, main () at main.c:66
66 uart_init();
(gdb) layout asm
(gdb) info registers
```

---

## 5. Discusión de resultados  

- El descifrado reproduce exactamente los bloques originales.  
- Los mensajes finales (`HOLA1234` y `"Mensaje de prueba para TEA"`) confirman la correcta implementación.  
- La salida UART es clara y sirve como evidencia directa.  
- GDB permite depuración paso a paso para verificar registros e instrucciones.  

---

## 6. Instrucciones para compilar, ejecutar y utilizar  

### Paso 1: Construir contenedor Docker  
```bash
chmod +x run.sh
./run.sh
```

### Paso 2: Compilar el proyecto dentro del contenedor  
```bash
cd /home/rvqemu-dev/workspace/tea-project
./build.sh
```

### Paso 3: Ejecutar en QEMU (ver salida UART en la terminal)  
```bash
./run-qemu.sh
```

### Paso 4: Depurar con GDB  
En otra terminal:
```bash
docker exec -it rvqemu /bin/bash
cd /home/rvqemu-dev/workspace/tea-project
gdb-multiarch tea.elf
```

### Paso 5: Conectar al servidor GDB  
```gdb
target remote :1234
break main  
continue
```
