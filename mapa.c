/*
 * mapa.c  –  BitQuest
 *
 * Unidad de traducción: definición del arreglo de mapa y funciones de
 * inicialización y renderizado.
 *
 * Dependencias:
 *   <stdio.h>  — printf(), putchar()
 *   <string.h> — disponible; no utilizado directamente en esta unidad
 *   mapa.h     — declara FILAS, COLUMNAS, CAM_FILAS, CAM_COLUMNAS y las
 *                constantes de carácter: CAMINO, MURO, JUGADOR, MONEDA,
 *                LLAVE, PUERTA, SALIDA.
 *
 * Arreglo principal:
 *   mapa[FILAS][COLUMNAS] — almacenamiento estático de duración de programa.
 *   Layout: row-major, acceso mapa[fila][col].
 *   Índices válidos: fila ∈ [0, FILAS-1], col ∈ [0, COLUMNAS-1].
 *   El borde exterior (fila 0, fila FILAS-1, col 0, col COLUMNAS-1) se
 *   reserva como MURO de contención en todos los niveles.
 */

#include <stdio.h>
#include <string.h>
#include "mapa.h"

/*
 * Definición del arreglo global del mapa.
 * Declarado extern en mapa.h; definido una sola vez aquí.
 * Tamaño en bytes: FILAS × COLUMNAS × sizeof(char) = 60 × 60 × 1 = 3600 bytes.
 * Inicializado a 0x00 por el runtime de C antes de la primera llamada a
 * inicializar_mapa(); ningún byte tiene valor semántico hasta esa llamada.
 */
char mapa[FILAS][COLUMNAS];


/* =============================================================================
 * FUNCIÓN: inicializar_mapa
 *
 * Entrada:
 *   nivel — entero que selecciona la geometría a escribir (1, 2, 3, 4).
 *           Valor no reconocido: la función completa los pasos comunes
 *           (limpieza y borde) y no escribe geometría interior adicional.
 *
 * Salida:
 *   void. Modifica mapa[][] en su totalidad.
 *
 * Operación en memoria/estado:
 *   1. Escribe CAMINO en todas las celdas [0..FILAS-1][0..COLUMNAS-1].
 *   2. Sobreescribe el borde exterior con MURO:
 *        columnas 0 y COLUMNAS-1 en todas las filas,
 *        filas 0 y FILAS-1 en todas las columnas.
 *   3. Según 'nivel', escribe la geometría específica (anillos, corredores,
 *      salas, tabiques, trampas) y coloca los objetos (LLAVE, PUERTA,
 *      SALIDA, MONEDA) en coordenadas fijas de diseño.
 *   4. Escribe JUGADOR en mapa[1][1] al final, independientemente del nivel.
 *
 * Invariantes de diseño comunes a todos los niveles:
 *   – La PUERTA ('D') es el único acceso a la sala que contiene la SALIDA ('E').
 *   – La LLAVE ('K') está siempre ubicada en una cámara sellada de acceso
 *     no trivial.
 *   – Las monedas se distribuyen en zonas que obligan a exploración completa.
 *   – Callejones sin salida ("trampas") no contienen LLAVE, PUERTA ni SALIDA.
 * =========================================================================== */
void inicializar_mapa(int nivel) {

    /* ── Paso 1: Limpieza total del arreglo ──────────────────────────────────
     * Escribe CAMINO (' ') en cada celda del arreglo 60×60.
     * Costo: FILAS × COLUMNAS = 3600 escrituras de un byte. */
    for (int f = 0; f < FILAS; f++)
        for (int c = 0; c < COLUMNAS; c++)
            mapa[f][c] = CAMINO;

    /* ── Paso 2: Borde exterior de contención ────────────────────────────────
     * Columnas 0 y COLUMNAS-1: MURO en cada fila → paredes laterales.
     * Filas 0 y FILAS-1: MURO en cada columna → paredes superior e inferior.
     * El borde impide que validar_movimiento() necesite comprobar < 0 / ≥ 60
     * para la mayoría de posiciones interiores; las esquinas se escriben dos
     * veces sin efecto semántico. */
    for (int f = 0; f < FILAS; f++) {
        mapa[f][0]          = MURO;   /* pared oeste */
        mapa[f][COLUMNAS-1] = MURO;   /* pared este  */
    }
    for (int c = 0; c < COLUMNAS; c++) {
        mapa[0][c]       = MURO;      /* pared norte */
        mapa[FILAS-1][c] = MURO;      /* pared sur   */
    }


    /* =========================================================================
     * NIVEL 1
     *
     * Geometría: cinco anillos concéntricos de muros con aperturas desplazadas,
     * más tabiques internos que rompen la simetría. El jugador debe atravesar
     * los anillos del exterior al interior para alcanzar la LLAVE y luego
     * retroceder hacia la bóveda de la SALIDA.
     *
     * Objetos:
     *   LLAVE  → mapa[55][4]   — cámara sellada, esquina inferior-izquierda.
     *   PUERTA → mapa[7][55]   — único acceso a la bóveda de la SALIDA.
     *   SALIDA → mapa[3][54]   — interior de la bóveda, esquina superior-este.
     *   MONEDA × 2:
     *     mapa[40][40] — zona sur-este, interior anillo 4.
     *     mapa[13][12] — zona norte-oeste, entre anillos 3 y 4.
     * ======================================================================= */
    if (nivel == 1) {

        /* ── Anillo 1: fila 2, fila 57, col 2, col 57 (rango col/fila 2..57) ─
         * Marco exterior de muros. Aperturas en posiciones asimétricas para
         * evitar alineación directa con el anillo siguiente. */
        for (int c = 2; c <= 57; c++) mapa[2][c]  = MURO;   /* lado norte */
        for (int c = 2; c <= 57; c++) mapa[57][c] = MURO;   /* lado sur   */
        for (int f = 2; f <= 57; f++) mapa[f][2]  = MURO;   /* lado oeste */
        for (int f = 2; f <= 57; f++) mapa[f][57] = MURO;   /* lado este  */
        /* Aperturas del anillo 1 */
        mapa[2][10]  = CAMINO;   /* norte, col 10  */
        mapa[57][40] = CAMINO;   /* sur,   col 40  */
        mapa[30][2]  = CAMINO;   /* oeste, fila 30 */
        mapa[30][57] = CAMINO;   /* este,  fila 30 */

        /* ── Anillo 2: fila 5, fila 54, col 5, col 54 (rango 5..54) ─────────
         * Ocho aperturas: dos por lado. Mayor densidad de pasos que el anillo 1
         * para permitir rutas alternativas en la zona media. */
        for (int c = 5; c <= 54; c++) mapa[5][c]  = MURO;
        for (int c = 5; c <= 54; c++) mapa[54][c] = MURO;
        for (int f = 5; f <= 54; f++) mapa[f][5]  = MURO;
        for (int f = 5; f <= 54; f++) mapa[f][54] = MURO;
        /* Aperturas del anillo 2 */
        mapa[5][20]  = CAMINO;   mapa[5][40]  = CAMINO;
        mapa[54][15] = CAMINO;   mapa[54][50] = CAMINO;
        mapa[25][5]  = CAMINO;   mapa[40][5]  = CAMINO;
        mapa[25][54] = CAMINO;   mapa[40][54] = CAMINO;

        /* ── Anillo 3: fila 8, fila 51, col 8, col 51 (rango 8..51) ─────────
         * Ocho aperturas. Encapsula los anillos 4 y 5. */
        for (int c = 8; c <= 51; c++) mapa[8][c]  = MURO;
        for (int c = 8; c <= 51; c++) mapa[51][c] = MURO;
        for (int f = 8; f <= 51; f++) mapa[f][8]  = MURO;
        for (int f = 8; f <= 51; f++) mapa[f][51] = MURO;
        /* Aperturas del anillo 3 */
        mapa[8][15]  = CAMINO;   mapa[8][35]  = CAMINO;
        mapa[51][20] = CAMINO;   mapa[51][45] = CAMINO;
        mapa[20][8]  = CAMINO;   mapa[38][8]  = CAMINO;
        mapa[20][51] = CAMINO;   mapa[38][51] = CAMINO;

        /* ── Anillo 4: fila 11, fila 48, col 11, col 48 (rango 11..48) ──────
         * Ocho aperturas. Contiene el anillo 5 y las MONEDAS. */
        for (int c = 11; c <= 48; c++) mapa[11][c] = MURO;
        for (int c = 11; c <= 48; c++) mapa[48][c] = MURO;
        for (int f = 11; f <= 48; f++) mapa[f][11] = MURO;
        for (int f = 11; f <= 48; f++) mapa[f][48] = MURO;
        /* Aperturas del anillo 4 */
        mapa[11][25] = CAMINO;   mapa[11][42] = CAMINO;
        mapa[48][18] = CAMINO;   mapa[48][38] = CAMINO;
        mapa[22][11] = CAMINO;   mapa[36][11] = CAMINO;
        mapa[22][48] = CAMINO;   mapa[36][48] = CAMINO;

        /* ── Anillo 5: fila 14, fila 45, col 14, col 45 (rango 14..45) ──────
         * Anillo más interior. Cuatro aperturas en posiciones desalineadas
         * con los pasos del anillo 4. */
        for (int c = 14; c <= 45; c++) mapa[14][c] = MURO;
        for (int c = 14; c <= 45; c++) mapa[45][c] = MURO;
        for (int f = 14; f <= 45; f++) mapa[f][14] = MURO;
        for (int f = 14; f <= 45; f++) mapa[f][45] = MURO;
        /* Aperturas del anillo 5 */
        mapa[14][28] = CAMINO;
        mapa[45][30] = CAMINO;
        mapa[28][14] = CAMINO;
        mapa[35][45] = CAMINO;

        /* ── Tabiques internos ───────────────────────────────────────────────
         * Muros parciales dentro del área del anillo 5; rompen la regularidad
         * y crean recodos sin crear cámaras cerradas. */

        /* Tabique horizontal fila 17, col 16..26; apertura en col 16 */
        for (int c = 16; c <= 26; c++) mapa[17][c] = MURO;
        mapa[17][16] = CAMINO;   /* paso oeste del tabique */

        /* Tabique vertical col 16, fila 16..26; apertura en fila 16 */
        for (int f = 16; f <= 26; f++) mapa[f][16] = MURO;
        mapa[16][16] = CAMINO;   /* paso norte del tabique */

        /* Tabique horizontal fila 29, col 27..33 +
         * tabique vertical col 30, fila 27..32;
         * la intersección [29][30] se abre para conectar ambos segmentos. */
        for (int c = 27; c <= 33; c++) mapa[29][c] = MURO;
        for (int f = 27; f <= 32; f++) mapa[f][30] = MURO;
        mapa[29][30] = CAMINO;   /* paso en la intersección del recodo */

        /* Tabique horizontal fila 43, col 38..47; apertura en col 47 */
        for (int c = 38; c <= 47; c++) mapa[43][c] = MURO;
        mapa[43][47] = CAMINO;   /* paso este del tabique */

        /* ── Monedas del nivel 1 (2 unidades) ───────────────────────────────
         * Ubicadas fuera del camino directo a la LLAVE; obligan al jugador a
         * explorar las zonas sur-este y norte-oeste antes de completar el nivel. */
        mapa[40][40] = MONEDA;   /* zona sur-este, interior anillo 4 */
        mapa[13][12] = MONEDA;   /* zona norte-oeste, entre anillos 3 y 4 */

        /* ── Cámara de la LLAVE — esquina inferior-izquierda ─────────────────
         * Cubículo sellado en filas 52..56, cols 3..7.
         * El bloque de MURO se escribe primero en su totalidad; luego se
         * abre el interior y la entrada única. */
        for (int f = 52; f <= 56; f++)
            for (int c = 3; c <= 7; c++)
                mapa[f][c] = MURO;            /* sellado total del cubículo */
        mapa[52][6] = CAMINO;                 /* única entrada: fila 52, col 6 */
        /* Interior habitable de la cámara: filas 53..56, cols 4..6 */
        for (int f = 53; f <= 56; f++)
            for (int c = 4; c <= 6; c++)
                mapa[f][c] = CAMINO;
        mapa[55][4] = LLAVE;                  /* LLAVE en la celda [55][4] */

        /* ── Bóveda de la SALIDA — esquina superior-derecha ──────────────────
         * Bloque de MURO en filas 2..7, cols 53..58.
         * Interior habitable: filas 3..6, cols 54..57.
         * Acceso único: PUERTA en mapa[7][55] (único hueco en fila 7).
         * Corredor de llegada horizontal en fila 8, cols 35..55: conecta la
         * apertura del anillo 3 ([8][35]) con la celda justo bajo la PUERTA. */
        for (int f = 2; f <= 7; f++)
            for (int c = 53; c <= 58; c++)
                mapa[f][c] = MURO;            /* sellado total de la bóveda */
        /* Interior habitable */
        for (int f = 3; f <= 6; f++)
            for (int c = 54; c <= 57; c++)
                mapa[f][c] = CAMINO;
        mapa[3][54] = SALIDA;                 /* SALIDA en el rincón interior */
        mapa[7][55] = PUERTA;                 /* único acceso; requiere LLAVE */
        /* Corredor horizontal que lleva a la entrada de la bóveda */
        for (int c = 35; c <= 55; c++) mapa[8][c] = CAMINO;

    } /* fin nivel 1 */


    /* =========================================================================
     * NIVEL 2
     *
     * Geometría: cuadrícula de 4×4 salas (16 celdas) separadas por muros
     * divisorios en filas 15, 30, 45 y columnas 15, 30, 45. Las salas se
     * identifican como (fila_bloque, col_bloque) con índices 0..3.
     * Las aperturas entre salas son puntuales (una celda por paso).
     *
     * Objetos:
     *   LLAVE  → mapa[54][4]   — interior de la cámara en sala (3,0).
     *   PUERTA → mapa[15][52]  — único acceso a la bóveda superior-derecha.
     *   SALIDA → mapa[3][57]   — interior de la bóveda, sala (0,3).
     *   MONEDA × 4:
     *     mapa[38][22] — sala (2,1), centro-sur.
     *     mapa[10][37] — sala (0,2), norte, accesible desde (0,1).
     *     mapa[52][22] — sala (3,1), obliga a explorar el sur.
     *     mapa[37][52] — sala (2,3), pasillo bajo la bóveda.
     * ======================================================================= */
    else if (nivel == 2) {

        /* ── Muros divisorios de la cuadrícula ──────────────────────────────
         * Tres muros verticales en cols 15, 30, 45 (filas 1..58).
         * Tres muros horizontales en filas 15, 30, 45 (cols 1..58).
         * Las esquinas del borde exterior ya son MURO por el paso 2 común. */
        for (int f = 1; f <= 58; f++) {
            mapa[f][15] = MURO;   /* separador vertical col 15 */
            mapa[f][30] = MURO;   /* separador vertical col 30 */
            mapa[f][45] = MURO;   /* separador vertical col 45 */
        }
        for (int c = 1; c <= 58; c++) {
            mapa[15][c] = MURO;   /* separador horizontal fila 15 */
            mapa[30][c] = MURO;   /* separador horizontal fila 30 */
            mapa[45][c] = MURO;   /* separador horizontal fila 45 */
        }

        /* ── Aperturas horizontales (entre columnas de salas) ────────────────
         * Cada apertura es una celda de CAMINO en el muro divisorio vertical.
         * Las conexiones marcadas CERRADO no tienen apertura: acceso imposible
         * sin atravesar otra sala primero. */
        mapa[7][15]  = CAMINO;   /* sala (0,0) ↔ sala (0,1) */
        mapa[7][30]  = CAMINO;   /* sala (0,1) ↔ sala (0,2) */
        /* (0,2) ↔ (0,3): CERRADO — bóveda sólo accesible por PUERTA desde (1,3) */

        mapa[22][15] = CAMINO;   /* sala (1,0) ↔ sala (1,1) */
        mapa[22][30] = CAMINO;   /* sala (1,1) ↔ sala (1,2) */
        mapa[22][45] = CAMINO;   /* sala (1,2) ↔ sala (1,3) */

        mapa[37][15] = CAMINO;   /* sala (2,0) ↔ sala (2,1) */
        mapa[37][30] = CAMINO;   /* sala (2,1) ↔ sala (2,2) */
        /* (2,2) ↔ (2,3): CERRADO */

        mapa[52][15] = CAMINO;   /* sala (3,0) ↔ sala (3,1) */
        mapa[52][30] = CAMINO;   /* sala (3,1) ↔ sala (3,2) */
        /* (3,2) ↔ (3,3): CERRADO — callejón sin salida real */

        /* ── Aperturas verticales (entre filas de salas) ─────────────────────
         * Cada apertura es una celda de CAMINO en el muro divisorio horizontal.
         * La columna de la apertura determina en qué zona de la sala queda el paso. */
        mapa[15][7]  = CAMINO;   /* sala (0,0) ↕ sala (1,0) */
        mapa[30][7]  = CAMINO;   /* sala (1,0) ↕ sala (2,0) */
        mapa[45][7]  = CAMINO;   /* sala (2,0) ↕ sala (3,0) — conduce a la LLAVE */

        mapa[15][22] = CAMINO;   /* sala (0,1) ↕ sala (1,1) */
        mapa[30][22] = CAMINO;   /* sala (1,1) ↕ sala (2,1) */
        mapa[45][22] = CAMINO;   /* sala (2,1) ↕ sala (3,1) */

        mapa[15][37] = CAMINO;   /* sala (0,2) ↕ sala (1,2) */
        mapa[29][37] = CAMINO;   /* sala (1,2) ↕ sala (2,2) — hueco en fila 29,
                                    una celda antes del muro divisorio fila 30 */
        mapa[44][37] = CAMINO;   /* sala (2,2) ↕ sala (3,2) */

        mapa[30][52] = CAMINO;   /* sala (1,3) ↕ sala (2,3) — acceso al callejón */

        /* Corredor interno en la columna 52, filas 16..29: conecta sala (1,3)
         * con la apertura vertical [30][52]. */
        for (int f = 16; f <= 29; f++) mapa[f][52] = CAMINO;

        /* ── Bóveda de la SALIDA — sala (0,3) ───────────────────────────────
         * Bloque sellado en filas 1..14, cols 46..58.
         * Interior habitable: filas 2..13, cols 47..57.
         * Acceso único: PUERTA en mapa[15][52].
         * mapa[14][52] = CAMINO: celda de tránsito inmediatamente al norte
         * de la PUERTA, dentro de la bóveda. */
        for (int f = 1; f <= 14; f++)
            for (int c = 46; c <= 58; c++)
                mapa[f][c] = MURO;            /* sellado total de la bóveda */
        /* Interior habitable */
        for (int f = 2; f <= 13; f++)
            for (int c = 47; c <= 57; c++)
                mapa[f][c] = CAMINO;
        mapa[3][57]  = SALIDA;                /* SALIDA en el rincón este */
        mapa[15][52] = PUERTA;                /* único acceso; requiere LLAVE */
        mapa[14][52] = CAMINO;                /* celda de tránsito al sur de la bóveda */

        /* ── Obstáculos internos en salas ────────────────────────────────────
         * Muros individuales que crean recodos dentro de salas seleccionadas.
         * No bloquean el acceso a ninguna sala; sólo aumentan la complejidad
         * de navegación local. */

        /* Sala (0,1): dos pilares de tres celdas y un pilar de dos celdas */
        mapa[4][19]  = MURO;   mapa[5][19]  = MURO;   mapa[6][19]  = MURO;
        mapa[4][25]  = MURO;   mapa[5][25]  = MURO;   mapa[6][25]  = MURO;
        mapa[9][20]  = MURO;   mapa[10][20] = MURO;   mapa[11][20] = MURO;
        mapa[9][26]  = MURO;   mapa[10][26] = MURO;

        /* Sala (1,1): dos tabiques horizontales con aperturas.
         * Tabique fila 19, cols 17..24; aperturas en cols 20 y 23.
         * Tabique fila 24, cols 17..28; apertura en col 26. */
        for (int c = 17; c <= 24; c++) mapa[19][c] = MURO;
        mapa[19][20] = CAMINO;   mapa[19][23] = CAMINO;
        for (int c = 17; c <= 28; c++) mapa[24][c] = MURO;
        mapa[24][26] = CAMINO;

        /* Sala (2,2): tabique horizontal fila 33 + tabique vertical col 42
         * + tabique horizontal fila 40; cada uno con una apertura. */
        for (int c = 33; c <= 42; c++) mapa[33][c] = MURO;
        mapa[33][38] = CAMINO;                         /* apertura norte */
        for (int f = 33; f <= 41; f++) mapa[f][42] = MURO;
        mapa[36][42] = CAMINO;                         /* apertura este  */
        for (int c = 35; c <= 42; c++) mapa[40][c] = MURO;
        mapa[40][35] = CAMINO;                         /* apertura oeste */

        /* Sala (1,0): seis muros individuales en dos grupos de fila;
         * crean obstáculos puntuales sin aislar zonas de la sala. */
        mapa[18][4]  = MURO;   mapa[18][5]  = MURO;   mapa[18][6]  = MURO;
        mapa[18][8]  = MURO;   mapa[18][9]  = MURO;   mapa[18][10] = MURO;
        mapa[23][4]  = MURO;   mapa[23][5]  = MURO;   mapa[23][6]  = MURO;

        /* ── Cámara de la LLAVE — sala (3,0) ────────────────────────────────
         * Cubículo interior sellado en filas 48..57, cols 3..13.
         * Muro de cierre superior: fila 48, cols 3..13; apertura única en col 8.
         * Muro de cierre inferior: fila 57, cols 3..13; apertura en col 7
         * (celda que queda bloqueada en la práctica por el borde, pero se
         * abre para consistencia de diseño).
         * Interior habitable: filas 49..56, cols 3..13. */
        for (int c = 3; c <= 13; c++) mapa[48][c] = MURO;
        mapa[48][8] = CAMINO;             /* única entrada superior */
        for (int f = 49; f <= 56; f++)
            for (int c = 3; c <= 13; c++)
                mapa[f][c] = CAMINO;      /* interior de la cámara */
        for (int c = 3; c <= 13; c++) mapa[57][c] = MURO;
        mapa[57][7] = CAMINO;             /* apertura en muro inferior */
        mapa[54][4] = LLAVE;              /* LLAVE en la celda [54][4] */

        /* ── Monedas del nivel 2 (4 unidades) ───────────────────────────────
         * Una por cada cuadrante de exploración; cada una obliga a alcanzar
         * una sala diferente de la cuadrícula. */
        mapa[38][22] = MONEDA;   /* sala (2,1): centro-sur                */
        mapa[10][37] = MONEDA;   /* sala (0,2): norte, accesible por (0,1)*/
        mapa[52][22] = MONEDA;   /* sala (3,1): obliga a descender al sur */
        mapa[37][52] = MONEDA;   /* sala (2,3): pasillo bajo la bóveda    */

    } /* fin nivel 2 */


    /* =========================================================================
     * NIVEL 3
     *
     * Geometría: laberinto orgánico de tres rutas divergentes desde una cámara
     * de inicio. El interior se inicializa completamente a MURO y se abren
     * únicamente los pasillos de diseño (técnica "esculpido").
     *
     * Rutas:
     *   NORTE — callejón sin salida real; contiene 2 monedas.
     *   ESTE  — conduce a la LLAVE (cámara E3 → corredor sur → [56][52]).
     *   SUR   — conduce a la PUERTA/SALIDA (sala S2 → corredor → [51][46]).
     *
     * Objetos:
     *   LLAVE  → mapa[56][52]  — cámara K, esquina inferior-este.
     *   PUERTA → mapa[50][46]  — único acceso a la bóveda sur.
     *   SALIDA → mapa[42][49]  — rincón NE de la bóveda.
     *   MONEDA × 6: dos por ruta (ver distribución en el bloque de monedas).
     * ======================================================================= */
    else if (nivel == 3) {

        /* ── Esculpido: relleno total con MURO ──────────────────────────────
         * Sobreescribe el paso 1 (CAMINO) en el interior del borde.
         * El borde exterior (fila 0, fila 59, col 0, col 59) permanece MURO
         * del paso 2; este bucle cubre filas/cols 1..58. */
        for (int f = 1; f <= 58; f++)
            for (int c = 1; c <= 58; c++)
                mapa[f][c] = MURO;

        /* ── Cámara de inicio (filas 1..6, cols 1..6) ───────────────────────
         * Área abierta inicial donde se posiciona el jugador en (1,1). */
        for (int f = 1; f <= 6; f++)
            for (int c = 1; c <= 6; c++)
                mapa[f][c] = CAMINO;

        /* ── Pasillo este → bifurcación A ────────────────────────────────────
         * Pasillo horizontal: fila 3, cols 7..15.
         * Pasillo vertical descendente: col 15, filas 3..14.
         * La bifurcación A (filas 10..14, cols 13..18) conecta las tres rutas. */
        for (int c = 7; c <= 15; c++) mapa[3][c] = CAMINO;   /* pasillo horizontal */
        for (int f = 3; f <= 14; f++) mapa[f][15] = CAMINO;  /* pasillo vertical   */

        /* Bifurcación A: nodo de distribución */
        for (int f = 10; f <= 14; f++)
            for (int c = 13; c <= 18; c++)
                mapa[f][c] = CAMINO;

        /* ── RUTA NORTE: callejón sin salida ─────────────────────────────────
         * Segmentos:
         *   col 13, filas 2..10  — bajada desde bifurcación hacia el norte.
         *   fila 2, cols 13..22  — pasillo techo norte.
         *   filas 2..5, cols 20..27 — ensanchamiento noroeste.
         *   fila 3, cols 27..35  — corredor al noreste.
         *   col 35, filas 3..9   — bajada al callejón norte profundo.
         * Ninguno de estos pasillos conecta con la LLAVE ni con la PUERTA. */
        for (int f = 2; f <= 10; f++)  mapa[f][13] = CAMINO;   /* subida norte     */
        for (int c = 13; c <= 22; c++) mapa[2][c]  = CAMINO;   /* techo norte      */
        for (int f = 2; f <= 5; f++)
            for (int c = 20; c <= 27; c++)
                mapa[f][c] = CAMINO;                            /* ensanchamiento   */
        for (int c = 27; c <= 35; c++) mapa[3][c]  = CAMINO;   /* corredor noreste */
        for (int f = 3; f <= 9; f++)   mapa[f][35] = CAMINO;   /* callejón profundo*/

        /* ── RUTA ESTE: conduce a la LLAVE ───────────────────────────────────
         * Pasillo fila 12, cols 18..28 → sala E1 (filas 10..16, cols 26..32).
         * Sub-callejón trampa: col 29 filas 4..10, fila 4 cols 27..35.
         * Continuación este: fila 13, cols 32..42 → col 42 filas 13..24.
         * Sala E2 (filas 22..28, cols 40..47).
         * Sub-callejón trampa oeste: fila 25 cols 30..40, col 30 filas 25..32.
         * Continuación sur-este: col 46 filas 28..38, fila 38 cols 46..55.
         * Sala E3 (filas 36..42, cols 53..58).
         * Corredor sur hacia K: col 55, filas 42..54.
         * Cámara K (filas 52..57, cols 52..57); muro interno en fila 53
         * con apertura en col 55; LLAVE en [56][52]. */
        for (int c = 18; c <= 28; c++) mapa[12][c] = CAMINO;   /* fila 12 → E1     */
        /* Sala E1 */
        for (int f = 10; f <= 16; f++)
            for (int c = 26; c <= 32; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón norte (trampa) desde E1 */
        for (int f = 4; f <= 10; f++)  mapa[f][29] = CAMINO;
        for (int c = 27; c <= 35; c++) mapa[4][c]  = CAMINO;
        /* Corredor este desde E1 */
        for (int c = 32; c <= 42; c++) mapa[13][c] = CAMINO;
        for (int f = 13; f <= 24; f++) mapa[f][42] = CAMINO;
        /* Sala E2 */
        for (int f = 22; f <= 28; f++)
            for (int c = 40; c <= 47; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón oeste (trampa) desde E2 */
        for (int c = 30; c <= 40; c++) mapa[25][c] = CAMINO;
        for (int f = 25; f <= 32; f++) mapa[f][30] = CAMINO;
        /* Corredor sur-este desde E2 hacia E3 */
        for (int f = 28; f <= 38; f++) mapa[f][46] = CAMINO;
        for (int c = 46; c <= 55; c++) mapa[38][c] = CAMINO;
        /* Sala E3 */
        for (int f = 36; f <= 42; f++)
            for (int c = 53; c <= 58; c++)
                mapa[f][c] = CAMINO;
        /* Corredor sur hacia cámara K */
        for (int f = 42; f <= 54; f++) mapa[f][55] = CAMINO;
        /* Cámara K */
        for (int f = 52; f <= 57; f++)
            for (int c = 52; c <= 57; c++)
                mapa[f][c] = CAMINO;
        /* Muro interno de la cámara K: fila 53, apertura en col 55 */
        for (int c = 52; c <= 57; c++) mapa[53][c] = MURO;
        mapa[53][55] = CAMINO;           /* paso único al interior de la cámara */
        mapa[56][52] = LLAVE;            /* LLAVE en la celda [56][52] */

        /* ── RUTA SUR: conduce a PUERTA/SALIDA ───────────────────────────────
         * Pasillo: col 15, filas 14..26.
         * Sala S1 (filas 24..30, cols 13..21).
         * Sub-callejón oeste (trampa): fila 27 cols 3..13, col 3 filas 27..35,
         *   fila 35 cols 3..9.
         * Corredor sur: col 17, filas 30..42.
         * Sala S2 (filas 40..48, cols 15..25).
         * Sub-callejón este (trampa): fila 43 cols 25..34, col 34 filas 43..50.
         * Corredor hacia PUERTA: col 20 filas 40..51, fila 51 cols 20..46.
         * Bóveda (filas 42..49, cols 42..49); SALIDA en [42][49];
         * PUERTA en [50][46]. */
        for (int f = 14; f <= 26; f++) mapa[f][15] = CAMINO;   /* bajada sur       */
        /* Sala S1 */
        for (int f = 24; f <= 30; f++)
            for (int c = 13; c <= 21; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón oeste (trampa) */
        for (int c = 3; c <= 13; c++)  mapa[27][c] = CAMINO;
        for (int f = 27; f <= 35; f++) mapa[f][3]  = CAMINO;
        for (int c = 3; c <= 9; c++)   mapa[35][c] = CAMINO;
        /* Corredor sur */
        for (int f = 30; f <= 42; f++) mapa[f][17] = CAMINO;
        /* Sala S2 */
        for (int f = 40; f <= 48; f++)
            for (int c = 15; c <= 25; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón este (trampa) */
        for (int c = 25; c <= 34; c++) mapa[43][c] = CAMINO;
        for (int f = 43; f <= 50; f++) mapa[f][34] = CAMINO;
        /* Corredor hacia PUERTA */
        for (int f = 40; f <= 51; f++) mapa[f][20] = CAMINO;
        for (int c = 20; c <= 46; c++) mapa[51][c] = CAMINO;
        /* Bóveda */
        for (int f = 42; f <= 49; f++)
            for (int c = 42; c <= 49; c++)
                mapa[f][c] = CAMINO;
        mapa[42][49] = SALIDA;           /* SALIDA en el rincón NE de la bóveda */
        mapa[50][46] = PUERTA;           /* único acceso a la bóveda; requiere LLAVE */

        /* Corredor decorativo adicional (conecta zona central con ruta sur) */
        for (int c = 20; c <= 30; c++) mapa[34][c] = CAMINO;
        for (int f = 28; f <= 34; f++) mapa[f][20] = CAMINO;
        for (int c = 18; c <= 26; c++) mapa[18][c] = CAMINO;
        for (int f = 15; f <= 22; f++) mapa[f][26] = CAMINO;

        /* ── Monedas del nivel 3 (6 unidades) ───────────────────────────────
         * Dos por cada ruta principal; dos de ellas en callejones trampa,
         * lo que obliga al jugador a explorarlos aunque no conduzcan a la meta. */
        mapa[3][23]  = MONEDA;   /* Ruta Norte: interior del ensanchamiento noroeste */
        mapa[6][35]  = MONEDA;   /* Ruta Norte: fondo del callejón norte profundo    */
        mapa[24][43] = MONEDA;   /* Ruta Este:  sala E2, col 42 de acceso            */
        mapa[47][34] = MONEDA;   /* Ruta Este:  sub-callejón trampa este desde E3    */
        mapa[44][18] = MONEDA;   /* Ruta Sur:   sala S2, centro                      */
        mapa[35][4]  = MONEDA;   /* Ruta Sur:   fondo del callejón oeste (trampa)    */

    } /* fin nivel 3 */


    /* =========================================================================
     * NIVEL 4
     *
     * Geometría: laberinto orgánico de alta densidad. Interior inicializado
     * completamente a MURO; los pasillos se abren por esculpido.
     *
     * Estructura:
     *   Cámara de inicio (filas 1..5, cols 1..8) → corredor este → núcleo.
     *   Núcleo (filas 2..10, cols 16..24): tabique vertical col 20 con paso
     *     único en fila 6.
     *   RUTA NORTE-ESTE: pasillo fila 2 → sala intermedia → corredor fila 6
     *     → cámara LLAVE (filas 3..8, cols 50..57).
     *   RUTA SUR: corredor col 18 → sala sur-A → meandro (filas 22..38,
     *     cols 24..46) → sala este-central → laberinto sur profundo
     *     (salas sur-B, sur-C) → bóveda final.
     *
     * Objetos:
     *   LLAVE  → mapa[7][53]   — cámara NE; muro interno en fila 5.
     *   PUERTA → mapa[50][46]  — único acceso a la bóveda final.
     *   SALIDA → mapa[55][53]  — interior de la bóveda, tras tabique fila 54.
     *   MONEDA × 8: dispersas en ruta norte, núcleo y laberinto sur.
     * ======================================================================= */
    else if (nivel == 4) {

        /* ── Esculpido: relleno total con MURO ──────────────────────────────*/
        for (int f = 1; f <= 58; f++)
            for (int c = 1; c <= 58; c++)
                mapa[f][c] = MURO;

        /* ── Cámara de inicio (filas 1..5, cols 1..8) ───────────────────────*/
        for (int f = 1; f <= 5; f++)
            for (int c = 1; c <= 8; c++)
                mapa[f][c] = CAMINO;

        /* ── Corredor de salida de la cámara (fila 3, cols 9..18) ───────────*/
        for (int c = 9; c <= 18; c++) mapa[3][c] = CAMINO;

        /* ── Núcleo central (filas 2..10, cols 16..24) ───────────────────────
         * Tabique vertical en col 20, filas 4..8; paso único en fila 6.
         * Divide el núcleo en sub-sala oeste (cols 16..19) y este (cols 21..24). */
        for (int f = 2; f <= 10; f++)
            for (int c = 16; c <= 24; c++)
                mapa[f][c] = CAMINO;
        for (int f = 4; f <= 8; f++) mapa[f][20] = MURO;
        mapa[6][20] = CAMINO;              /* único paso entre sub-salas del núcleo */

        /* ── RUTA NORTE-ESTE ──────────────────────────────────────────────────
         * Pasillo horizontal: fila 2, cols 24..38.
         * Bajada: col 38, filas 2..8.
         * Sala intermedia (filas 4..9, cols 36..44).
         * Callejón trampa norte: col 40 filas 1..4, fila 1 cols 38..45.
         * Corredor este hacia K: fila 6, cols 44..54.
         * Cámara K (filas 3..8, cols 50..57); muro interno en fila 5
         *   con apertura en col 53; LLAVE en [7][53]. */
        for (int c = 24; c <= 38; c++) mapa[2][c] = CAMINO;     /* pasillo norte    */
        for (int f = 2; f <= 8;  f++) mapa[f][38] = CAMINO;     /* bajada           */
        /* Sala intermedia norte */
        for (int f = 4; f <= 9;  f++)
            for (int c = 36; c <= 44; c++)
                mapa[f][c] = CAMINO;
        /* Callejón trampa norte */
        for (int f = 1; f <= 4;  f++) mapa[f][40] = CAMINO;
        for (int c = 38; c <= 45; c++) mapa[1][c] = CAMINO;     /* techo trampa     */
        /* Corredor hacia cámara K */
        for (int c = 44; c <= 54; c++) mapa[6][c] = CAMINO;
        /* Cámara K */
        for (int f = 3; f <= 8;  f++)
            for (int c = 50; c <= 57; c++)
                mapa[f][c] = CAMINO;
        /* Muro interno de la cámara K */
        for (int c = 50; c <= 57; c++) mapa[5][c] = MURO;
        mapa[5][53] = CAMINO;             /* único paso al interior de la cámara K */
        mapa[7][53] = LLAVE;              /* LLAVE en la celda [7][53] */

        /* ── CORREDOR PRINCIPAL SUR ───────────────────────────────────────────
         * col 18, filas 10..18 — desciende del núcleo al laberinto sur. */
        for (int f = 10; f <= 18; f++) mapa[f][18] = CAMINO;

        /* ── Sala sur-A (filas 14..20, cols 14..26) ──────────────────────────
         * Tabique horizontal: fila 17, cols 14..22; aperturas en cols 16 y 21. */
        for (int f = 14; f <= 20; f++)
            for (int c = 14; c <= 26; c++)
                mapa[f][c] = CAMINO;
        /* Tabique interno de sala sur-A */
        for (int c = 14; c <= 22; c++) mapa[17][c] = MURO;
        mapa[17][16] = CAMINO;            /* paso norte del tabique */
        mapa[17][21] = CAMINO;            /* paso sur del tabique   */

        /* Callejón oeste falso (trampa) desde sala sur-A */
        for (int c = 4; c <= 14; c++)  mapa[15][c] = CAMINO;    /* entrada trampa   */
        for (int f = 15; f <= 22; f++) mapa[f][4]  = CAMINO;    /* pasillo oeste    */
        for (int c = 4; c <= 10; c++)  mapa[22][c] = CAMINO;    /* fondo sin salida */

        /* ── ZONA CENTRAL-ESTE: laberinto meandro (filas 22..38, cols 24..46) ─
         * Bajada desde sala sur-A: col 24, filas 20..28.
         * Meandro 1: fila 22 cols 24..36 → col 36 filas 16..22 → fila 16 cols 28..36.
         *   Callejón norte trampa: col 32 filas 10..16, fila 10 cols 30..36.
         * Meandro 2: col 36 filas 22..30 → fila 30 cols 28..40 → col 40 filas 24..30
         *   → fila 24 cols 36..44.
         * Sala este-central (filas 22..32, cols 42..50); tabique vertical
         *   col 46, filas 24..30; paso único en fila 27.
         * Callejón este falso (trampa): fila 26 cols 50..57, col 57 filas 20..26,
         *   fila 20 cols 50..57. */
        for (int f = 20; f <= 28; f++) mapa[f][24] = CAMINO;    /* bajada meandro   */
        /* Meandro 1 */
        for (int c = 24; c <= 36; c++) mapa[22][c] = CAMINO;
        for (int f = 16; f <= 22; f++) mapa[f][36] = CAMINO;
        for (int c = 28; c <= 36; c++) mapa[16][c] = CAMINO;
        /* Callejón norte trampa desde techo del meandro 1 */
        for (int f = 10; f <= 16; f++) mapa[f][32] = CAMINO;
        for (int c = 30; c <= 36; c++) mapa[10][c] = CAMINO;
        /* Meandro 2 */
        for (int f = 22; f <= 30; f++) mapa[f][36] = CAMINO;
        for (int c = 28; c <= 40; c++) mapa[30][c] = CAMINO;
        for (int f = 24; f <= 30; f++) mapa[f][40] = CAMINO;
        for (int c = 36; c <= 44; c++) mapa[24][c] = CAMINO;
        /* Sala este-central */
        for (int f = 22; f <= 32; f++)
            for (int c = 42; c <= 50; c++)
                mapa[f][c] = CAMINO;
        /* Tabique interno sala este-central */
        for (int f = 24; f <= 30; f++) mapa[f][46] = MURO;
        mapa[27][46] = CAMINO;            /* único paso interior del tabique */
        /* Callejón este falso (trampa) */
        for (int c = 50; c <= 57; c++) mapa[26][c] = CAMINO;
        for (int f = 20; f <= 26; f++) mapa[f][57] = CAMINO;
        for (int c = 50; c <= 57; c++) mapa[20][c] = CAMINO;

        /* ── LABERINTO SUR PROFUNDO (filas 28..49) ───────────────────────────
         * Corredor sur: col 28, filas 28..40.
         * Sala sur-B (filas 34..42, cols 22..34); tabique fila 38 cols 22..30
         *   con pasos en cols 25 y 29.
         * Callejón oeste profundo (trampa): fila 36 cols 6..22, col 6 filas 36..46,
         *   fila 46 cols 6..16, col 16 filas 42..46.
         * Corredor sur-este desde sala sur-B: fila 42, cols 28..42.
         * Sala sur-C (filas 36..48, cols 38..48); tabique vertical col 42,
         *   filas 38..44; paso único en fila 41.
         * Corredor hacia PUERTA: col 46, filas 48..50. */
        for (int f = 28; f <= 40; f++) mapa[f][28] = CAMINO;    /* corredor sur     */
        /* Sala sur-B */
        for (int f = 34; f <= 42; f++)
            for (int c = 22; c <= 34; c++)
                mapa[f][c] = CAMINO;
        /* Tabique sala sur-B */
        for (int c = 22; c <= 30; c++) mapa[38][c] = MURO;
        mapa[38][25] = CAMINO;            /* paso norte del tabique */
        mapa[38][29] = CAMINO;            /* paso sur del tabique   */
        /* Callejón oeste profundo (trampa) */
        for (int c = 6; c <= 22; c++)  mapa[36][c] = CAMINO;
        for (int f = 36; f <= 46; f++) mapa[f][6]  = CAMINO;
        for (int c = 6; c <= 16; c++)  mapa[46][c] = CAMINO;
        for (int f = 42; f <= 46; f++) mapa[f][16] = CAMINO;
        /* Corredor sur-este */
        for (int c = 28; c <= 42; c++) mapa[42][c] = CAMINO;
        /* Sala sur-C */
        for (int f = 36; f <= 48; f++)
            for (int c = 38; c <= 48; c++)
                mapa[f][c] = CAMINO;
        /* Tabique interno sala sur-C */
        for (int c = 38; c <= 44; c++) mapa[42][c] = CAMINO;    /* fila ya abierta  */
        for (int f = 38; f <= 44; f++) mapa[f][42] = MURO;
        mapa[41][42] = CAMINO;            /* único paso del recodo */
        /* Corredor hacia PUERTA */
        for (int f = 48; f <= 50; f++) mapa[f][46] = CAMINO;

        /* ── BÓVEDA FINAL (filas 51..58, cols 43..57) ────────────────────────
         * Muro norte de contención: fila 50 (permanece MURO del esculpido).
         * Interior habitable: filas 51..57, cols 43..57.
         * Tabique interno: fila 54, cols 43..57; paso único en col 53.
         * SALIDA en [55][53]: accesible sólo tras cruzar la PUERTA y el tabique.
         * PUERTA en [50][46]: único hueco en fila 50 entre cols 43 y 57. */
        for (int f = 51; f <= 57; f++)
            for (int c = 43; c <= 57; c++)
                mapa[f][c] = CAMINO;      /* interior de la bóveda */
        /* Tabique interno de la bóveda */
        for (int c = 43; c <= 57; c++) mapa[54][c] = MURO;
        mapa[54][53] = CAMINO;            /* paso único hacia la celdilla final */
        mapa[55][53] = SALIDA;            /* SALIDA al fondo del pasillo        */
        mapa[50][46] = PUERTA;            /* único acceso a la bóveda; requiere LLAVE */

        /* ── Monedas del nivel 4 (8 unidades) ───────────────────────────────
         * Distribuidas en todas las zonas del laberinto.
         * M1 y M2 en ruta norte; M3 y M4 en el núcleo/sur-A;
         * M5–M8 en el laberinto sur profundo. */
        mapa[6][40]  = MONEDA;   /* ruta norte, sala intermedia              */
        mapa[1][44]  = MONEDA;   /* ruta norte, techo del callejón trampa    */
        mapa[7][17]  = MONEDA;   /* núcleo central, sub-sala oeste           */
        mapa[19][22] = MONEDA;   /* sala sur-A, zona sur                     */
        mapa[27][44] = MONEDA;   /* sala este-central, lado oeste del tabique*/
        mapa[23][57] = MONEDA;   /* callejón este (trampa), celda transitable*/
        mapa[35][30] = MONEDA;   /* sala sur-B, zona norte                   */
        mapa[45][46] = MONEDA;   /* sala sur-C, rincón accesible             */

    } /* fin nivel 4 */


    /* ── Posición inicial del jugador ────────────────────────────────────────
     * Se escribe al final para sobreescribir cualquier carácter colocado
     * previamente en [1][1] por la geometría del nivel. En todos los niveles,
     * [1][1] es una celda de CAMINO al inicio del área habitable. */
    mapa[1][1] = JUGADOR;
}


/* =============================================================================
 * FUNCIÓN: dibujar_mapa
 *
 * Entrada:
 *   m         — arreglo char[FILAS][COLUMNAS] a renderizar (puede ser mapa[][]).
 *   cam_fila  — fila de inicio de la ventana de cámara (antes del ajuste).
 *   cam_col   — columna de inicio de la ventana de cámara (antes del ajuste).
 *
 * Salida:
 *   void. Escribe secuencias ANSI y caracteres en stdout.
 *
 * Operación en memoria/estado:
 *   1. Satura cam_fila y cam_col a los límites válidos para que la ventana
 *      de tamaño CAM_FILAS × CAM_COLUMNAS no exceda el arreglo.
 *   2. Itera sobre la subventana [cam_fila .. cam_fila+CAM_FILAS-1]
 *                                [cam_col  .. cam_col +CAM_COLUMNAS-1].
 *   3. Para cada celda, selecciona un código de color ANSI y un símbolo
 *      mediante switch; emite la secuencia ESC[<n>m<char>ESC[0m.
 *   4. Emite '\n' al final de cada fila de la ventana.
 *
 * Formato de secuencias ANSI:
 *   \033[<n>m  — establece atributo de color; \033[0m restablece al default.
 *   Códigos utilizados: 90 (gris oscuro), 37 (blanco), 96 (cian brillante),
 *   93 (amarillo brillante), 92 (verde brillante), 31 (rojo), 95 (magenta).
 * =========================================================================== */
void dibujar_mapa(char m[FILAS][COLUMNAS], int cam_fila, int cam_col) {

    /* ── Ajuste de límites de la cámara ──────────────────────────────────────
     * Satura los valores negativos a 0.
     * Satura los valores que desplazarían la ventana fuera del arreglo al
     * máximo desplazamiento válido. */
    if (cam_fila < 0)                       cam_fila = 0;
    if (cam_col  < 0)                       cam_col  = 0;
    if (cam_fila > FILAS    - CAM_FILAS)    cam_fila = FILAS    - CAM_FILAS;
    if (cam_col  > COLUMNAS - CAM_COLUMNAS) cam_col  = COLUMNAS - CAM_COLUMNAS;

    /* ── Iteración sobre la ventana de cámara ────────────────────────────────
     * Acceso: m[fila][col]; fila y col dentro de los límites saturados. */
    for (int fila = cam_fila; fila < cam_fila + CAM_FILAS; fila++) {
        for (int col = cam_col; col < cam_col + CAM_COLUMNAS; col++) {
            char celda = m[fila][col];   /* byte de la celda actual */

            switch (celda) {
                case MURO:      /* bloque sólido U+2588 (█), gris oscuro \033[90m */
                    printf("\033[90m\xE2\x96\x88\033[0m");
                    break;
                case CAMINO:    /* punto ASCII '.', blanco \033[37m             */
                    printf("\033[37m.\033[0m");
                    break;
                case JUGADOR:   /* cian brillante \033[96m       */
                    printf("\033[96m%c\033[0m", celda);
                    break;
                case MONEDA:    /* amarillo brillante \033[93m    */
                    printf("\033[93m%c\033[0m", celda);
                    break;
                case LLAVE:     /* verde brillante \033[92m        */
                    printf("\033[92m%c\033[0m", celda);
                    break;
                case PUERTA:    /* rojo \033[31m                  */
                    printf("\033[31m%c\033[0m", celda);
                    break;
                case SALIDA:    /* magenta brillante \033[95m     */
                    printf("\033[95m%c\033[0m", celda);
                    break;
                default:        /* Carácter no mapeado: sin modificar el color  */
                    putchar(celda);
                    break;
            }
        }
        putchar('\n');   /* fin de línea de la ventana; avanza al siguiente renglón */
    }
}