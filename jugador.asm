section .text

    global mover_jugador
    global es_muro_asm

    extern jugador_fila
    extern jugador_col
    extern jugador_tiene_llave
    extern jugador_gano
    extern mapa

; int es_muro_asm(char celda)
; Recibe el char en CL (parte baja de RCX)
; Retorna 1 si es '#', 0 si no
es_muro_asm:
    xor     eax, eax            ; eax = 0 (asumir no es muro)
    cmp     cl, '#'
    jne     .no_es_muro
    mov     eax, 1              ; eax = 1 (es muro)
.no_es_muro:
    ret

; void mover_jugador(char tecla)
mover_jugador:
    push    rbx
    push    rsi
    push    rdi

    movsx   rax, cl
    mov     ebx, [rel jugador_fila]
    mov     esi, [rel jugador_col]
    mov     edi, ebx
    mov     edx, esi

    cmp     rax, 'w'
    jne     .check_s
    dec     edi
    jmp     .validar

.check_s:
    cmp     rax, 's'
    jne     .check_a
    inc     edi
    jmp     .validar

.check_a:
    cmp     rax, 'a'
    jne     .check_d
    dec     edx
    jmp     .validar

.check_d:
    cmp     rax, 'd'
    jne     .fin
    inc     edx

.validar:
    cmp     edi, 0
    jl      .fin
    cmp     edi, 60
    jge     .fin
    cmp     edx, 0
    jl      .fin
    cmp     edx, 60
    jge     .fin

    movsx   rax, edi
    imul    rax, 60
    movsx   r10, edx
    add     rax, r10
    lea     r8, [rel mapa]
    movsx   r9, byte [r8 + rax]

    ; --- Usar es_muro_asm para verificar colisión ---
    mov     rcx, r9
    call    es_muro_asm
    cmp     eax, 1
    je      .fin

    cmp     r9, 'D'
    jne     .check_salida
    cmp     dword [rel jugador_tiene_llave], 0
    je      .fin

.check_salida:
    cmp     r9, 'S'
    jne     .check_llave
    cmp     dword [rel jugador_tiene_llave], 0
    je      .fin

.check_llave:
    cmp     r9, 'K'
    jne     .check_gano
    mov     dword [rel jugador_tiene_llave], 1

.check_gano:
    cmp     r9, 'S'
    jne     .mover
    mov     dword [rel jugador_gano], 1

.mover:
    movsx   rax, ebx
    imul    rax, 60
    movsx   r10, esi
    add     rax, r10
    mov     byte [r8 + rax], ' '

    mov     [rel jugador_fila], edi
    mov     [rel jugador_col],  edx

    movsx   rax, edi
    imul    rax, 60
    movsx   r10, edx
    add     rax, r10
    mov     byte [r8 + rax], 'P'

.fin:
    pop     rdi
    pop     rsi
    pop     rbx
    ret