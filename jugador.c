#include "mapa.h"
#include "jugador.h"


/* =============================================================================
 * BLOQUE 1: DEFINICIÓN DE VARIABLES DE ESTADO
 *
 * Almacenamiento estático de duración de programa (linkage externo).
 * Declaradas extern en jugador.h; definidas aquí una sola vez.
 * Inicializadas a los valores de inicio de partida.
 * =========================================================================== */

int jugador_fila        = 1;   /* Fila inicial del jugador. Posición [1][1] evita
                                   el borde de muros que rodea el mapa.          */
int jugador_col         = 1;   /* Columna inicial del jugador.                   */
int jugador_tiene_llave = 0;   /* Sin llave al inicio de cada nivel.             */
int jugador_gano        = 0;   /* Condición de victoria inactiva al inicio.      */
int jugador_monedas     = 0;   /* Cero monedas recogidas al inicio de nivel.     */
int monedas_nivel       = 1;   /* Mínimo 1; sobreescrito por main.c al cargar el
                                   nivel mediante contar_caracter_mapa().         */
int jugador_pasos       = 0;   /* Acumulador global; no se reinicia por nivel.   */
int jugador_puntaje     = 0;   /* Sobreescrito por main.c al invocar
                                   calcular_puntaje() al término de cada nivel.  */
int jugador_niveles     = 0;   /* Incrementado por main.c al detectar
                                   jugador_gano == 1.                            */


/* =============================================================================
 * FUNCIÓN: mover_jugador
 *
 * Entrada:
 *   tecla — carácter de dirección: 'w' (norte), 's' (sur), 'a' (oeste),
 *            'd' (este). Cualquier otro valor produce retorno inmediato.
 *
 * Salida:
 *   void. Modifica el estado global y el arreglo mapa[][] según el resultado
 *   de la validación.
 *
 * Operación en memoria/estado:
 *   1. Calcula (f_nueva, c_nueva) aplicando el delta de dirección sobre
 *      (jugador_fila, jugador_col).
 *   2. Delega la verificación de transitabilidad física a validar_movimiento()
 *      (rutinas.asm). Aborta si retorna 0.
 *   3. Evalúa el contenido de la celda destino mediante detectar_objeto_celda()
 *      (rutinas.asm) para cada tipo de objeto relevante, en orden de precedencia:
 *        PUERTA  → bloquea si jugador_tiene_llave == 0.
 *        LLAVE   → establece jugador_tiene_llave = 1.
 *        MONEDA  → incrementa jugador_monedas.
 *        SALIDA  → establece jugador_gano = 1.
 *   4. Escribe CAMINO en mapa[jugador_fila][jugador_col] (celda de origen).
 *   5. Actualiza jugador_fila y jugador_col a (f_nueva, c_nueva).
 *   6. Escribe JUGADOR en mapa[jugador_fila][jugador_col] (celda destino).
 *
 * Nota: jugador_pasos no se incrementa en esta implementación. Si se requiere,
 *       debe añadirse tras la confirmación del movimiento (paso 4).
 * =========================================================================== */
void mover_jugador(char tecla) {

    /* Copias locales de la posición actual; preservan el origen hasta confirmar
       el movimiento. No se modifican las globales hasta la validación completa. */
    int f_nueva = jugador_fila;
    int c_nueva = jugador_col;

    /* Cálculo del delta de posición según la tecla recibida.
       Retorno inmediato si 'tecla' no corresponde a ninguna dirección válida. */
    if      (tecla == 'w') f_nueva--;   /* Norte: fila − 1 */
    else if (tecla == 's') f_nueva++;   /* Sur:   fila + 1 */
    else if (tecla == 'a') c_nueva--;   /* Oeste: col  − 1 */
    else if (tecla == 'd') c_nueva++;   /* Este:  col  + 1 */
    else return;                        /* Tecla no reconocida; no hay movimiento */

    /* Verificación de transitabilidad física (colisión con '#' y límites de mapa).
       El cast (char*) aplana el arreglo 2D mapa[][] a puntero lineal para la ABI
       de rutinas.asm, que opera sobre arreglo row-major de char. */
    if (validar_movimiento((char*)mapa, COLUMNAS, f_nueva, c_nueva) == 0) return;

    /* Evaluación de PUERTA ('P').
       Si la celda destino es puerta y el jugador no posee la llave, el movimiento
       queda bloqueado aunque validar_movimiento() lo haya aprobado (la puerta no
       es '#', por lo que pasa la validación física). */
    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, PUERTA)) {
        if (!jugador_tiene_llave) return;   /* jugador_tiene_llave == 0: bloqueado */
    }

    /* Evaluación de LLAVE ('L').
       El flag se activa antes de actualizar la posición; la llave permanece
       consumida del mapa al sobreescribirse la celda con JUGADOR en el paso final. */
    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, LLAVE)) {
        jugador_tiene_llave = 1;            /* jugador_tiene_llave ← 1 */
    }

    /* Evaluación de MONEDA ('$').
       jugador_monedas se incrementa antes de reubicar al jugador; la celda
       pierde el símbolo '$' al escribirse JUGADOR sobre ella. */
    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, MONEDA)) {
        jugador_monedas++;                  /* jugador_monedas ← jugador_monedas + 1 */
    }

    /* Evaluación de SALIDA ('S').
       Establece la condición de victoria; main.c detecta jugador_gano == 1
       en el bucle principal para iniciar la secuencia de fin de nivel.
       El jugador se posiciona sobre 'S' antes de que main.c procese la salida. */
    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, SALIDA)) {
        jugador_gano = 1;                   /* jugador_gano ← 1 */
    }

    /* Actualización del arreglo mapa[][]:
       La celda de origen recupera el valor CAMINO (' '); el símbolo del jugador
       no persiste en posiciones ya abandonadas. */
    mapa[jugador_fila][jugador_col] = CAMINO;   /* mapa[fila_anterior][col_anterior] ← ' ' */

    /* Confirmación de la nueva posición en las variables globales. */
    jugador_fila = f_nueva;                     /* jugador_fila ← f_nueva */
    jugador_col  = c_nueva;                     /* jugador_col  ← c_nueva */

    /* La celda destino toma el valor JUGADOR, haciendo visible la posición
       actual en el renderizado del mapa por main.c / render.c. */
    mapa[jugador_fila][jugador_col] = JUGADOR;  /* mapa[f_nueva][c_nueva] ← JUGADOR */
}