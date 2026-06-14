#ifndef JUGADOR_H
#define JUGADOR_H

/* ── Variables de estado del jugador ──────────────────────────────────────── */
extern int jugador_fila;
extern int jugador_col;
extern int jugador_tiene_llave;
extern int jugador_gano;
extern int jugador_monedas;     /* monedas recogidas en el nivel actual       */
extern int monedas_nivel;       /* cuántas se requieren (lo fija main.c)      */

/* ── Variables nuevas requeridas por las funciones ASM ────────────────────── */
extern int jugador_pasos;       /* pasos acumulados en la partida             */
extern int jugador_puntaje;     /* puntaje calculado por calcular_puntaje()   */
extern int jugador_niveles;     /* niveles completados (actualizado en main.c)*/

/* ── Función de movimiento principal ─────────────────────────────────────── */
void mover_jugador(char tecla);

/* ── Auxiliar de compatibilidad (original) ───────────────────────────────── */
int  es_muro_asm(char celda);

/* ── FUNCIÓN 1: Contar caracteres en el mapa ─────────────────────────────── *
 * Recorre el arreglo lineal del mapa y cuenta cuántas celdas contienen       *
 * el carácter indicado. Uso principal: contar monedas ('$') al cargar nivel. *
 *                                                                             *
 * Parámetros:                                                                 *
 *   mapa_ptr     – puntero al inicio del mapa (char *)                       *
 *   total_celdas – número total de celdas (60*60 = 3600)                     *
 *   car          – carácter a buscar                                          *
 * Retorna: cantidad de coincidencias (int)                                    */
int contar_caracter_mapa(char *mapa_ptr, int total_celdas, char car);

/* ── FUNCIÓN 2: Validar movimiento ──────────────────────────────────────────*
 * Determina si la celda (fila, col) es transitable (no es muro '#').         *
 *                                                                             *
 * Parámetros:                                                                 *
 *   mapa_ptr – puntero al inicio del mapa                                    *
 *   columnas – número de columnas (60)                                        *
 *   fila     – fila  propuesta                                                *
 *   col      – columna propuesta                                              *
 * Retorna: 1 = válido (libre / moneda / llave / puerta / salida)             *
 *          0 = bloqueado (muro o fuera de límites)                           */
int validar_movimiento(char *mapa_ptr, int columnas, int fila, int col);

/* ── FUNCIÓN 3: Calcular puntaje ─────────────────────────────────────────── *
 * Fórmula: (monedas * 100) + (niveles * 500) - pasos                        *
 *                                                                             *
 * Parámetros:                                                                 *
 *   monedas – monedas totales recolectadas                                   *
 *   pasos   – total de pasos realizados                                       *
 *   niveles – niveles completados                                             *
 * Retorna: puntaje final (int, puede ser negativo)                           */
int calcular_puntaje(int monedas, int pasos, int niveles);

/* ── FUNCIÓN 4: Detectar objeto en una celda ─────────────────────────────── *
 * Comprueba si la celda (fila, col) contiene exactamente el carácter dado.   *
 *                                                                             *
 * Parámetros:                                                                 *
 *   mapa_ptr – puntero al inicio del mapa                                    *
 *   columnas – número de columnas (60)                                        *
 *   fila     – fila  a revisar                                                *
 *   col      – columna a revisar                                              *
 *   objeto   – carácter a buscar ('L', 'P', 'S', '$', etc.)                 *
 * Retorna: 1 si la celda coincide, 0 si no                                   */
int detectar_objeto_celda(char *mapa_ptr, int columnas,
                          int fila, int col, char objeto);

/* ── FUNCIÓN 5: Contar celdas libres ─────────────────────────────────────── *
 * Cuenta cuántas celdas del mapa son camino libre (' ').                     *
 *                                                                             *
 * Parámetros:                                                                 *
 *   mapa_ptr     – puntero al inicio del mapa                                *
 *   total_celdas – número total de celdas (3600)                             *
 * Retorna: número de celdas con espacio ' ' (int)                            */
int contar_celdas_libres(char *mapa_ptr, int total_celdas);

#endif /* JUGADOR_H */