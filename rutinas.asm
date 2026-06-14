; =============================================================================
; jugador.asm  –  BitQuest  |  x86-64 Windows ABI (fastcall)
; Convención: RCX, RDX, R8, R9  |  Shadow space 32 bytes antes de cada CALL
;             RAX, RCX, RDX, R8, R9, R10, R11  → volátiles (no preservar)
;             RBX, RSI, RDI, RBP, R12-R15       → no volátiles (preservar)
; =============================================================================

section .text

    ; ── Símbolos exportados ───────────────────────────────────────────────
    global contar_caracter_mapa     ; Función 1
    global validar_movimiento       ; Función 2
    global calcular_puntaje         ; Función 3
    global detectar_objeto_celda    ; Función 4
    global contar_celdas_libres     ; Función 5
    global es_muro_asm              ; Auxiliar (mantiene compatibilidad)

    ; ── Variables externas de C ───────────────────────────────────────────
    extern jugador_fila
    extern jugador_col
    extern jugador_tiene_llave
    extern jugador_gano
    extern jugador_monedas
    extern monedas_nivel
    extern mapa
    extern jugador_pasos            ; NUEVO: contador de pasos
    extern jugador_puntaje          ; NUEVO: puntaje calculado
    extern jugador_niveles          ; NUEVO: niveles completados

; =============================================================================
; FUNCIÓN 1: contar_caracter_mapa
; Prototipo C: int contar_caracter_mapa(char *mapa, int total_celdas, char car);
;
; Propósito : Recorre el arreglo lineal del mapa y cuenta cuántas celdas
;             contienen el carácter solicitado. Se usa para contabilizar
;             monedas ('$') al cargar un nivel sin "quemar" el valor en C.
;
; Parámetros (Windows x64 fastcall):
;   RCX  = dirección base del mapa  (char *)
;   EDX  = total de celdas          (int, 3600 para mapa 60×60)
;   R8B  = carácter a buscar        (char)
;
; Retorno:
;   EAX  = cantidad de coincidencias
; =============================================================================
contar_caracter_mapa:
    ; No usamos registros no volátiles → sin push/pop necesario
    xor     eax, eax            ; contador = 0
    xor     r10d, r10d          ; índice i  = 0
    movzx   r11d, r8b           ; r11d = carácter buscado (sin signo)
    test    edx, edx
    jle     .ccm_fin            ; total <= 0 → salir

.ccm_bucle:
    cmp     r10d, edx           ; i < total_celdas ?
    jge     .ccm_fin
    movzx   r9d, byte [rcx + r10]   ; celda actual (sin signo)
    cmp     r9d, r11d
    jne     .ccm_siguiente
    inc     eax                 ; coincidencia → incrementar contador

.ccm_siguiente:
    inc     r10d
    jmp     .ccm_bucle

.ccm_fin:
    ret

; =============================================================================
; FUNCIÓN 2: validar_movimiento
; Prototipo C: int validar_movimiento(char *mapa, int columnas, int fila, int col);
;
; Propósito : Determina si la celda (fila, col) es transitable.
;             Reemplaza la validación inline de mover_jugador.
;
; Parámetros:
;   RCX  = dirección base del mapa
;   EDX  = número de columnas (60)
;   R8D  = fila  propuesta
;   R9D  = columna propuesta
;
; Retorno:
;   EAX  = 1 → movimiento válido (camino libre, moneda, llave, puerta abierta, salida)
;           0 → bloqueado (muro '#' o fuera de límites)
;
; Nota: Esta función NO evalúa si el jugador tiene llave; esa lógica de alto
;       nivel sigue en mover_jugador, que llama aquí solo para la colisión física.
; =============================================================================
validar_movimiento:
    ; Verificar límites: 0 <= fila < 60  y  0 <= col < 60
    xor     eax, eax            ; asumir inválido
    cmp     r8d, 0
    jl      .vm_invalido
    cmp     r8d, 60
    jge     .vm_invalido
    cmp     r9d, 0
    jl      .vm_invalido
    cmp     r9d, 60
    jge     .vm_invalido

    ; Calcular índice lineal: idx = fila * columnas + col
    mov     eax, r8d            ; eax = fila
    imul    eax, edx            ; eax = fila * columnas
    add     eax, r9d            ; eax += col
    movsx   rax, eax            ; extender a 64 bits (índice seguro)
    movzx   r10d, byte [rcx + rax]   ; celda = mapa[fila][col]

    ; Verificar si es MURO
    xor     eax, eax            ; asumir 0 (bloqueado)
    cmp     r10d, '#'
    je      .vm_invalido        ; es muro → retornar 0

    mov     eax, 1              ; cualquier otro carácter → válido

.vm_invalido:
    ret

; =============================================================================
; FUNCIÓN 3: calcular_puntaje
; Prototipo C: int calcular_puntaje(int monedas, int pasos, int niveles);
;
; Propósito : Aplica la fórmula de puntuación íntegramente en ensamblador.
;             Fórmula: (monedas * 100) + (niveles * 500) - pasos
;
; Parámetros:
;   ECX  = monedas recolectadas (total acumulado)
;   EDX  = pasos realizados
;   R8D  = niveles completados
;
; Retorno:
;   EAX  = puntaje final (puede ser negativo si pasos es muy alto)
; =============================================================================
calcular_puntaje:
    ; Parte 1: monedas * 100
    mov     eax, ecx
    imul    eax, 100            ; eax = monedas * 100

    ; Parte 2: niveles * 500
    mov     r10d, r8d
    imul    r10d, 500           ; r10d = niveles * 500

    ; Parte 3: sumar y restar pasos
    add     eax, r10d           ; eax = (monedas*100) + (niveles*500)
    sub     eax, edx            ; eax -= pasos

    ret

; =============================================================================
; FUNCIÓN 4: detectar_objeto_celda
; Prototipo C: int detectar_objeto_celda(char *mapa, int columnas,
;                                        int fila, int col, char objeto);
;
; Propósito : Comprueba si una celda específica contiene exactamente el
;             carácter buscado ('L', 'P', 'S', '$', etc.).
;
; Parámetros:
;   RCX  = dirección base del mapa
;   EDX  = número de columnas (60)
;   R8D  = fila  a revisar
;   R9D  = columna a revisar
;   [pila+40] = carácter del objeto (5.º parámetro, tras shadow space de 32 bytes
;               y dirección de retorno de 8 bytes → RSP+40 al entrar a la función)
;
; Retorno:
;   EAX  = 1 si la celda coincide, 0 si no
;
; Convención Windows x64: el 5.º argumento entero va en la PILA.
; El caller reserva 32 bytes de shadow space, por lo que al entrar aquí:
;   [RSP+0]  = dirección de retorno
;   [RSP+8]  = shadow para RCX  (no usado por nosotros)
;   [RSP+16] = shadow para RDX
;   [RSP+24] = shadow para R8
;   [RSP+32] = shadow para R9
;   [RSP+40] = 5.º argumento (char objeto, pasado como int de 32 bits)
; =============================================================================
detectar_objeto_celda:
    ; Leer el 5.º parámetro de la pila
    movzx   r10d, byte [rsp+40]     ; r10d = objeto a buscar

    ; Verificar límites
    xor     eax, eax
    cmp     r8d, 0
    jl      .doc_no
    cmp     r8d, 60
    jge     .doc_no
    cmp     r9d, 0
    jl      .doc_no
    cmp     r9d, 60
    jge     .doc_no

    ; Calcular índice y leer celda
    mov     eax, r8d
    imul    eax, edx
    add     eax, r9d
    movsx   rax, eax
    movzx   r11d, byte [rcx + rax]  ; r11d = celda actual

    ; Comparar con el objeto buscado
    xor     eax, eax
    cmp     r11d, r10d
    jne     .doc_no
    mov     eax, 1

.doc_no:
    ret

; =============================================================================
; FUNCIÓN 5: contar_celdas_libres
; Prototipo C: int contar_celdas_libres(char *mapa, int total_celdas);
;
; Propósito : Cuenta cuántas celdas del mapa son camino libre (' ').
;             Útil para estadísticas y verificaciones de nivel.
;
; Parámetros:
;   RCX  = dirección base del mapa
;   EDX  = total de celdas (3600 para 60×60)
;
; Retorno:
;   EAX  = número de celdas con carácter espacio (' ')
; =============================================================================
contar_celdas_libres:
    xor     eax, eax            ; contador = 0
    xor     r10d, r10d          ; índice   = 0
    test    edx, edx
    jle     .ccl_fin

.ccl_bucle:
    cmp     r10d, edx
    jge     .ccl_fin
    movzx   r11d, byte [rcx + r10]
    cmp     r11d, ' '           ; ¿es camino libre?
    jne     .ccl_siguiente
    inc     eax

.ccl_siguiente:
    inc     r10d
    jmp     .ccl_bucle

.ccl_fin:
    ret

; =============================================================================
; AUXILIAR: es_muro_asm
; Prototipo C: int es_muro_asm(char celda);
;
; Mantiene compatibilidad con jugador.h existente.
; Parámetros:
;   CL   = carácter a evaluar
; Retorno:
;   EAX  = 1 si es '#', 0 si no
; =============================================================================
es_muro_asm:
    xor     eax, eax
    cmp     cl, '#'
    jne     .em_no_muro
    mov     eax, 1
.em_no_muro:
    ret
