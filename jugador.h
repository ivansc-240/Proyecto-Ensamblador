#ifndef JUGADOR_H
#define JUGADOR_H

/* =============================================================================
 * BLOQUE 1: VARIABLES DE ESTADO DEL JUGADOR
 *
 * Definidas en jugador.c (o main.c según la unidad de traducción activa).
 * Declaradas extern para enlace externo entre unidades de compilación.
 * Todas de tipo int; ocupan 4 bytes en almacenamiento estático.
 * =========================================================================== */

extern int jugador_fila;        /* Fila actual del jugador en el mapa [0, 59].
                                   Modificada por mover_jugador() en cada paso
                                   vertical válido.                            */

extern int jugador_col;         /* Columna actual del jugador en el mapa [0, 59].
                                   Modificada por mover_jugador() en cada paso
                                   horizontal válido.                          */

extern int jugador_tiene_llave; /* Flag de posesión de llave.
                                   0 = no posee llave.
                                   1 = llave recogida; habilita el paso por 'D'
                                   (puerta) en mover_jugador().                */

extern int jugador_gano;        /* Flag de condición de victoria.
                                   0 = partida en curso.
                                   1 = jugador alcanzó la celda 'E' con llave;
                                   activa la secuencia de fin de nivel en main.c.*/

extern int jugador_monedas;     /* Monedas recogidas en el nivel actual.
                                   Incrementado por mover_jugador() al pisar '$'.
                                   Se reinicia al iniciar cada nivel.          */

extern int monedas_nivel;       /* Total de monedas presentes en el nivel.
                                   Asignado por main.c mediante
                                   contar_caracter_mapa() al cargar el mapa.
                                   No se modifica durante el juego.            */

extern int jugador_pasos;       /* Contador acumulado de pasos desde el inicio
                                   de la partida (no se reinicia por nivel).
                                   Incrementado por mover_jugador() en cada
                                   desplazamiento efectivo. Usado como
                                   penalización en calcular_puntaje().         */

extern int jugador_puntaje;     /* Resultado de la última llamada a
                                   calcular_puntaje(). Actualizado por main.c
                                   al completar cada nivel o al finalizar la
                                   partida.                                    */

extern int jugador_niveles;     /* Contador de niveles completados.
                                   Incrementado por main.c al detectar
                                   jugador_gano == 1. Usado como factor
                                   positivo en calcular_puntaje().             */


/* =============================================================================
 * BLOQUE 2: FUNCIÓN DE MOVIMIENTO
 * Implementación: jugador.c
 * =========================================================================== */

/*
 * mover_jugador
 *
 * Entrada : tecla — carácter de dirección ('w','a','s','d' o equivalentes).
 * Salida  : ninguna (void). Modifica el estado global del jugador.
 * Efecto  : Calcula la celda destino a partir de (jugador_fila, jugador_col)
 *           y el vector asociado a 'tecla'. Invoca validar_movimiento() para
 *           verificar transitabilidad. Si la celda es válida, actualiza
 *           jugador_fila / jugador_col, incrementa jugador_pasos e
 *           interpreta el contenido de la celda destino ('M', 'K', 'D', 'E').
 */
void mover_jugador(char tecla);


/* =============================================================================
 * BLOQUE 3: FUNCIONES IMPLEMENTADAS EN rutinas.asm (x86-64 Windows ABI)
 *
 * Convención de llamada: RCX, RDX, R8, R9 para los cuatro primeros argumentos.
 * Argumentos adicionales (5.º en adelante) se pasan en pila tras 32 bytes de
 * shadow space reservados por el llamador.
 * Valor de retorno: EAX.
 * =========================================================================== */

/*
 * es_muro_asm
 *
 * Entrada : celda — byte del carácter a evaluar (pasado en CL, parte baja RCX).
 * Salida  : 1 si celda == '#' (0x23); 0 en cualquier otro caso.
 * Efecto  : Ningún estado global es modificado.
 * Nota    : Declarada para compatibilidad con unidades que referencian este
 *           símbolo directamente. La lógica equivalente está integrada en
 *           validar_movimiento().
 */
int es_muro_asm(char celda);

/*
 * contar_caracter_mapa
 *
 * Entrada : mapa_ptr     — dirección base del arreglo lineal del mapa (char *).
 *           total_celdas — número de celdas a examinar; 3600 para mapa 60×60.
 *           car          — carácter a contabilizar (p.ej. '$').
 * Salida  : Número de ocurrencias de 'car' en mapa_ptr[0 .. total_celdas-1].
 * Efecto  : Lectura de solo lectura sobre mapa_ptr. Ningún estado global
 *           es modificado. Uso principal: determinar monedas_nivel al cargar
 *           un nivel, invocando contar_caracter_mapa(mapa, 3600, '$').
 */
int contar_caracter_mapa(char *mapa_ptr, int total_celdas, char car);

/*
 * validar_movimiento
 *
 * Entrada : mapa_ptr — dirección base del mapa (char *).
 *           columnas — número de columnas por fila; 60.
 *           fila     — fila propuesta para el movimiento [0, 59].
 *           col      — columna propuesta para el movimiento [0, 59].
 * Salida  : 1 si la celda (fila, col) es transitable (distinta de '#' y dentro
 *             de límites).
 *           0 si la celda es '#', o si fila/col están fuera del rango [0, 59].
 * Efecto  : Lectura de solo lectura sobre mapa_ptr. Ningún estado global
 *           es modificado. La evaluación de posesión de llave para traspasar
 *           puertas ('D') no está contemplada aquí; reside en mover_jugador().
 */
int validar_movimiento(char *mapa_ptr, int columnas, int fila, int col);

/*
 * calcular_puntaje
 *
 * Entrada : monedas — total de monedas recolectadas (acumulado de todos los niveles).
 *           pasos   — total de pasos realizados (penalización).
 *           niveles — niveles completados (bonificación).
 * Salida  : Resultado entero de: (monedas × 100) + (niveles × 500) − pasos.
 *           El valor puede ser negativo si pasos supera los términos positivos.
 * Efecto  : Sin acceso a memoria externa. Sin modificación de estado global.
 *           El llamador es responsable de asignar el resultado a jugador_puntaje.
 */
int calcular_puntaje(int monedas, int pasos, int niveles);

/*
 * detectar_objeto_celda
 *
 * Entrada : mapa_ptr — dirección base del mapa (char *).
 *           columnas — número de columnas; 60.
 *           fila     — fila a inspeccionar [0, 59].
 *           col      — columna a inspeccionar [0, 59].
 *           objeto   — carácter a comparar ('K', 'D', 'E', 'M', '#', etc.).
 *                      5.º argumento; pasado en pila a [RSP+40] en el llamado.
 * Salida  : 1 si mapa[fila * columnas + col] == objeto y las coordenadas son
 *             válidas.
 *           0 si no coincide o las coordenadas están fuera de límites.
 * Efecto  : Lectura de solo lectura sobre mapa_ptr. Ningún estado global
 *           es modificado.
 */
int detectar_objeto_celda(char *mapa_ptr, int columnas,
                          int fila, int col, char objeto);

/*
 * contar_celdas_libres
 *
 * Entrada : mapa_ptr     — dirección base del mapa (char *).
 *           total_celdas — número de celdas a examinar; 3600 para mapa 60×60.
 * Salida  : Número de celdas en mapa_ptr[0 .. total_celdas-1] cuyo valor
 *           es 0x20 (carácter espacio ' ').
 * Efecto  : Lectura de solo lectura sobre mapa_ptr. Ningún estado global
 *           es modificado. Uso principal: estadísticas de nivel y verificación
 *           de densidad de caminos transitables.
 */
int contar_celdas_libres(char *mapa_ptr, int total_celdas);

#endif /* JUGADOR_H */