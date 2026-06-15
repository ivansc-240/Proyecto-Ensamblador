section .text

    ; Símbolos exportados — visibles al enlazador externo (C runtime / MSVC/GCC)
    global contar_caracter_mapa     ; Función 1
    global validar_movimiento       ; Función 2
    global calcular_puntaje         ; Función 3
    global detectar_objeto_celda    ; Función 4
    global contar_celdas_libres     ; Función 5
    global es_muro_asm              ; Auxiliar: mantiene compatibilidad con jugador.h

    ; Variables globales definidas en la unidad de traducción C (.c).
    ; Acceso mediante direccionamiento de memoria relativo a RIP en tiempo de enlace.
    extern jugador_fila             ; int  — fila actual del jugador en el mapa
    extern jugador_col              ; int  — columna actual del jugador en el mapa
    extern jugador_tiene_llave      ; int  — flag: 1 si posee la llave, 0 si no
    extern jugador_gano             ; int  — flag: 1 si alcanzó la salida con llave
    extern jugador_monedas          ; int  — monedas recolectadas en el nivel actual
    extern monedas_nivel            ; int  — total de monedas presentes en el nivel
    extern mapa                     ; char[] — arreglo lineal row-major del mapa
    extern jugador_pasos            ; int  — contador acumulado de pasos realizados
    extern jugador_puntaje          ; int  — puntaje calculado tras aplicar fórmula
    extern jugador_niveles          ; int  — contador de niveles completados

; =============================================================================
; FUNCIÓN 1: contar_caracter_mapa
; Prototipo C: int contar_caracter_mapa(char *mapa, int total_celdas, char car);
;
; Entrada:
;   RCX  — dirección base del arreglo lineal del mapa (char *)
;   EDX  — número total de celdas a examinar (int; 3600 para mapa 60×60)
;   R8B  — carácter a contar (char; p.ej. '$' para monedas)
;
; Salida:
;   EAX  — número de ocurrencias del carácter en el rango [0, total_celdas)
;
; Operación en registros:
;   EAX  acumula el conteo de coincidencias.
;   R10D mantiene el índice de iteración (i).
;   R11D almacena el carácter objetivo en extensión sin signo.
;   R9D  almacena temporalmente cada byte leído del mapa.
;
; Preservación: ningún registro no volátil es modificado; no se requiere
;               prólogo/epílogo de pila.
; =============================================================================
contar_caracter_mapa:
    xor     eax, eax            ; EAX ← 0  (contador de coincidencias)
    xor     r10d, r10d          ; R10D ← 0 (índice i)
    movzx   r11d, r8b           ; R11D ← zero-extend(R8B): carácter objetivo sin signo
    test    edx, edx            ; FLAGS ← EDX AND EDX
    jle     .ccm_fin            ; EDX ≤ 0 → no hay celdas que examinar; saltar al retorno

.ccm_bucle:
    cmp     r10d, edx           ; FLAGS ← R10D − EDX
    jge     .ccm_fin            ; i ≥ total_celdas → bucle terminado
    movzx   r9d, byte [rcx + r10]   ; R9D ← zero-extend(mapa[i]): byte en índice i
    cmp     r9d, r11d           ; FLAGS ← R9D − R11D
    jne     .ccm_siguiente      ; byte ≠ objetivo → no contar
    inc     eax                 ; EAX ← EAX + 1: coincidencia registrada

.ccm_siguiente:
    inc     r10d                ; R10D ← R10D + 1 (i++)
    jmp     .ccm_bucle          ; siguiente iteración

.ccm_fin:
    ret                         ; retorna con EAX = conteo total

; =============================================================================
; FUNCIÓN 2: validar_movimiento
; Prototipo C: int validar_movimiento(char *mapa, int columnas, int fila, int col);
;
; Entrada:
;   RCX  — dirección base del mapa (char *)
;   EDX  — número de columnas por fila (int; 60 para mapa 60×60)
;   R8D  — fila propuesta para el movimiento (int)
;   R9D  — columna propuesta para el movimiento (int)
;
; Salida:
;   EAX  = 1 → celda transitable (espacio libre, moneda, llave, puerta abierta, salida)
;   EAX  = 0 → celda bloqueada (carácter '#') o coordenada fuera del mapa
;
; Operación en registros:
;   R10D almacena el byte leído de la celda evaluada.
;   RAX  se usa como índice lineal de 64 bits tras extensión de signo.
;
; Nota: la evaluación de la posesión de llave para traspasar puertas no
;       reside en esta función; corresponde al llamador (mover_jugador).
; =============================================================================
validar_movimiento:
    xor     eax, eax            ; EAX ← 0 (asumir movimiento inválido)

    ; Verificación de límites: 0 ≤ fila < 60
    cmp     r8d, 0              ; FLAGS ← R8D − 0
    jl      .vm_invalido        ; fila < 0 → fuera del mapa
    cmp     r8d, 60             ; FLAGS ← R8D − 60
    jge     .vm_invalido        ; fila ≥ 60 → fuera del mapa

    ; Verificación de límites: 0 ≤ col < 60
    cmp     r9d, 0              ; FLAGS ← R9D − 0
    jl      .vm_invalido        ; col < 0 → fuera del mapa
    cmp     r9d, 60             ; FLAGS ← R9D − 60
    jge     .vm_invalido        ; col ≥ 60 → fuera del mapa

    ; Cálculo del índice lineal: idx = fila * columnas + col
    mov     eax, r8d            ; EAX ← fila
    imul    eax, edx            ; EAX ← fila * columnas  (producto de 32 bits)
    add     eax, r9d            ; EAX ← (fila * columnas) + col
    movsx   rax, eax            ; RAX ← sign-extend(EAX): índice seguro a 64 bits
                                ;        evita wrap-around en aritmética de punteros
    movzx   r10d, byte [rcx + rax]  ; R10D ← zero-extend(mapa[idx]): byte de la celda

    ; Evaluación del contenido de la celda
    xor     eax, eax            ; EAX ← 0 (asumir bloqueado)
    cmp     r10d, '#'           ; FLAGS ← R10D − 0x23
    je      .vm_invalido        ; celda es muro → retornar 0

    mov     eax, 1              ; cualquier carácter distinto de '#' → válido

.vm_invalido:
    ret                         ; retorna con EAX = 0 (inválido) o 1 (válido)

; =============================================================================
; FUNCIÓN 3: calcular_puntaje
; Prototipo C: int calcular_puntaje(int monedas, int pasos, int niveles);
;
; Entrada:
;   ECX  — monedas recolectadas (acumulado total)
;   EDX  — pasos realizados (penalización)
;   R8D  — niveles completados
;
; Salida:
;   EAX  — puntaje resultante; puede ser negativo si pasos > monedas*100 + niveles*500
;
; Fórmula aplicada:
;   puntaje = (monedas × 100) + (niveles × 500) − pasos
;
; Operación en registros:
;   EAX  acumula los términos positivos de la fórmula.
;   R10D calcula el término (niveles × 500) de forma independiente antes de sumarse.
;   EDX  se substrae al final como penalización por pasos.
;
; Preservación: ningún registro no volátil es modificado.
; =============================================================================
calcular_puntaje:
    ; Término 1: monedas × 100
    mov     eax, ecx            ; EAX ← monedas
    imul    eax, 100            ; EAX ← monedas × 100

    ; Término 2: niveles × 500
    mov     r10d, r8d           ; R10D ← niveles
    imul    r10d, 500           ; R10D ← niveles × 500

    ; Acumulación y penalización
    add     eax, r10d           ; EAX ← (monedas×100) + (niveles×500)
    sub     eax, edx            ; EAX ← suma_términos_positivos − pasos

    ret                         ; retorna con EAX = puntaje final

; =============================================================================
; FUNCIÓN 4: detectar_objeto_celda
; Prototipo C: int detectar_objeto_celda(char *mapa, int columnas,
;                                        int fila, int col, char objeto);
;
; Entrada:
;   RCX      — dirección base del mapa (char *)
;   EDX      — número de columnas (int; 60)
;   R8D      — fila a evaluar (int)
;   R9D      — columna a evaluar (int)
;   [RSP+40] — 5.º argumento: carácter del objeto a buscar (char, pasado como int)
;              Disposición de la pila al entrar (sin prólogo):
;                [RSP+ 0] = dirección de retorno (8 bytes)
;                [RSP+ 8] = shadow space para RCX
;                [RSP+16] = shadow space para RDX
;                [RSP+24] = shadow space para R8
;                [RSP+32] = shadow space para R9
;                [RSP+40] = 5.º argumento (primer parámetro en pila)
;
; Salida:
;   EAX  = 1 si mapa[fila][col] == objeto
;   EAX  = 0 si la celda no coincide o las coordenadas están fuera de límites
;
; Operación en registros:
;   R10D almacena el carácter objetivo leído de la pila.
;   R11D almacena el byte leído de la celda del mapa.
;   RAX  se usa como índice lineal de 64 bits.
;
; Preservación: ningún registro no volátil es modificado.
; =============================================================================
detectar_objeto_celda:
    ; Lectura del 5.º parámetro desde la pila (Windows x64: primer arg en pila)
    movzx   r10d, byte [rsp+40]     ; R10D ← zero-extend(*(RSP+40)): carácter objetivo

    ; Verificación de límites: 0 ≤ fila < 60
    xor     eax, eax                ; EAX ← 0 (asumir no encontrado)
    cmp     r8d, 0                  ; FLAGS ← R8D − 0
    jl      .doc_no                 ; fila < 0 → fuera del mapa
    cmp     r8d, 60                 ; FLAGS ← R8D − 60
    jge     .doc_no                 ; fila ≥ 60 → fuera del mapa

    ; Verificación de límites: 0 ≤ col < 60
    cmp     r9d, 0                  ; FLAGS ← R9D − 0
    jl      .doc_no                 ; col < 0 → fuera del mapa
    cmp     r9d, 60                 ; FLAGS ← R9D − 60
    jge     .doc_no                 ; col ≥ 60 → fuera del mapa

    ; Cálculo del índice lineal: idx = fila * columnas + col
    mov     eax, r8d                ; EAX ← fila
    imul    eax, edx                ; EAX ← fila * columnas
    add     eax, r9d                ; EAX ← (fila * columnas) + col
    movsx   rax, eax                ; RAX ← sign-extend(EAX): índice de 64 bits
    movzx   r11d, byte [rcx + rax]  ; R11D ← zero-extend(mapa[idx]): byte de la celda

    ; Comparación celda vs. objeto buscado
    xor     eax, eax                ; EAX ← 0 (asumir no coincidencia)
    cmp     r11d, r10d              ; FLAGS ← R11D − R10D
    jne     .doc_no                 ; ≠ → celda no contiene el objeto
    mov     eax, 1                  ; EAX ← 1: coincidencia confirmada

.doc_no:
    ret                             ; retorna con EAX = 0 o 1

; =============================================================================
; FUNCIÓN 5: contar_celdas_libres
; Prototipo C: int contar_celdas_libres(char *mapa, int total_celdas);
;
; Entrada:
;   RCX  — dirección base del mapa (char *)
;   EDX  — total de celdas a examinar (int; 3600 para mapa 60×60)
;
; Salida:
;   EAX  — número de celdas cuyo byte es 0x20 (carácter espacio ' ')
;
; Operación en registros:
;   EAX  acumula el conteo de celdas libres.
;   R10D mantiene el índice de iteración (i).
;   R11D almacena el byte leído de cada celda.
;
; Preservación: ningún registro no volátil es modificado.
; =============================================================================
contar_celdas_libres:
    xor     eax, eax            ; EAX ← 0 (contador de celdas libres)
    xor     r10d, r10d          ; R10D ← 0 (índice i)
    test    edx, edx            ; FLAGS ← EDX AND EDX
    jle     .ccl_fin            ; EDX ≤ 0 → sin celdas que examinar

.ccl_bucle:
    cmp     r10d, edx           ; FLAGS ← R10D − EDX
    jge     .ccl_fin            ; i ≥ total_celdas → fin de bucle
    movzx   r11d, byte [rcx + r10]  ; R11D ← zero-extend(mapa[i])
    cmp     r11d, '.'           ; FLAGS ← R11D − 0x20
    jne     .ccl_siguiente      ; ≠ espacio → no contar
    inc     eax                 ; EAX ← EAX + 1: celda libre registrada

.ccl_siguiente:
    inc     r10d                ; R10D ← R10D + 1 (i++)
    jmp     .ccl_bucle          ; siguiente iteración

.ccl_fin:
    ret                         ; retorna con EAX = total de celdas libres

; =============================================================================
; AUXILIAR: es_muro_asm
; Prototipo C: int es_muro_asm(char celda);
;
; Entrada:
;   CL   — byte del carácter a evaluar (primer parámetro: parte baja de RCX)
;
; Salida:
;   EAX  = 1 si CL == '#' (0x23)
;   EAX  = 0 en caso contrario
;
; Operación en registros:
;   EAX  recibe el resultado booleano directamente.
;
; Propósito: mantiene compatibilidad con declaraciones previas en jugador.h
;            que referencian este símbolo por nombre.
; Preservación: ningún registro no volátil es modificado.
; =============================================================================
es_muro_asm:
    xor     eax, eax            ; EAX ← 0 (asumir que no es muro)
    cmp     cl, '#'             ; FLAGS ← CL − 0x23
    jne     .em_no_muro         ; CL ≠ '#' → saltar, EAX permanece 0
    mov     eax, 1              ; EAX ← 1: el carácter es muro

.em_no_muro:
    ret                         ; retorna con EAX = 0 o 1