#include <stdio.h>
#include <string.h>
#include "mapa.h"

char mapa[FILAS][COLUMNAS];

void inicializar_mapa(int nivel) {
 
    /* ── 1. Limpiar todo con caminos libres ─────────────────────────────── */
    for (int f = 0; f < FILAS; f++)
        for (int c = 0; c < COLUMNAS; c++)
            mapa[f][c] = CAMINO;
 
    /* ── 2. Borde exterior de seguridad ─────────────────────────────────── */
    for (int f = 0; f < FILAS; f++) {
        mapa[f][0]          = MURO;
        mapa[f][COLUMNAS-1] = MURO;
    }
    for (int c = 0; c < COLUMNAS; c++) {
        mapa[0][c]       = MURO;
        mapa[FILAS-1][c] = MURO;
    }

    if (nivel == 1) {
 
        /* ── Anillos ──────────────────────────────────────────────────────── */
        /* Anillo 1 */
        for (int c = 2; c <= 57; c++) mapa[2][c]  = MURO;
        for (int c = 2; c <= 57; c++) mapa[57][c] = MURO;
        for (int f = 2; f <= 57; f++) mapa[f][2]  = MURO;
        for (int f = 2; f <= 57; f++) mapa[f][57] = MURO;
        mapa[2][10]  = CAMINO; mapa[57][40] = CAMINO;
        mapa[30][2]  = CAMINO; mapa[30][57] = CAMINO;
 
        /* Anillo 2 */
        for (int c = 5; c <= 54; c++) mapa[5][c]  = MURO;
        for (int c = 5; c <= 54; c++) mapa[54][c] = MURO;
        for (int f = 5; f <= 54; f++) mapa[f][5]  = MURO;
        for (int f = 5; f <= 54; f++) mapa[f][54] = MURO;
        mapa[5][20]  = CAMINO; mapa[5][40]  = CAMINO;
        mapa[54][15] = CAMINO; mapa[54][50] = CAMINO;
        mapa[25][5]  = CAMINO; mapa[40][5]  = CAMINO;
        mapa[25][54] = CAMINO; mapa[40][54] = CAMINO;
 
        /* Anillo 3 */
        for (int c = 8; c <= 51; c++) mapa[8][c]  = MURO;
        for (int c = 8; c <= 51; c++) mapa[51][c] = MURO;
        for (int f = 8; f <= 51; f++) mapa[f][8]  = MURO;
        for (int f = 8; f <= 51; f++) mapa[f][51] = MURO;
        mapa[8][15]  = CAMINO; mapa[8][35]  = CAMINO;
        mapa[51][20] = CAMINO; mapa[51][45] = CAMINO;
        mapa[20][8]  = CAMINO; mapa[38][8]  = CAMINO;
        mapa[20][51] = CAMINO; mapa[38][51] = CAMINO;
 
        /* Anillo 4 */
        for (int c = 11; c <= 48; c++) mapa[11][c] = MURO;
        for (int c = 11; c <= 48; c++) mapa[48][c] = MURO;
        for (int f = 11; f <= 48; f++) mapa[f][11] = MURO;
        for (int f = 11; f <= 48; f++) mapa[f][48] = MURO;
        mapa[11][25] = CAMINO; mapa[11][42] = CAMINO;
        mapa[48][18] = CAMINO; mapa[48][38] = CAMINO;
        mapa[22][11] = CAMINO; mapa[36][11] = CAMINO;
        mapa[22][48] = CAMINO; mapa[36][48] = CAMINO;
 
        /* Anillo 5 */
        for (int c = 14; c <= 45; c++) mapa[14][c] = MURO;
        for (int c = 14; c <= 45; c++) mapa[45][c] = MURO;
        for (int f = 14; f <= 45; f++) mapa[f][14] = MURO;
        for (int f = 14; f <= 45; f++) mapa[f][45] = MURO;
        mapa[14][28] = CAMINO; mapa[45][30] = CAMINO;
        mapa[28][14] = CAMINO; mapa[35][45] = CAMINO;
 
        /* ── Tabiques internos ──────────────────────────────────────────── */
        for (int c = 16; c <= 26; c++) mapa[17][c] = MURO;
        mapa[17][16] = CAMINO;
 
        for (int f = 16; f <= 26; f++) mapa[f][16] = MURO;
        mapa[16][16] = CAMINO;
 
        for (int c = 27; c <= 33; c++) mapa[29][c] = MURO;
        for (int f = 27; f <= 32; f++) mapa[f][30] = MURO;
        mapa[29][30] = CAMINO;
 
        for (int c = 38; c <= 47; c++) mapa[43][c] = MURO;
        mapa[43][47] = CAMINO;
 
        /* ── Cámara LLAVE K – esquina inferior-izquierda ─────────────────── */
        /* Cubículo sellado en (52..56 , 3..7), entrada única fila 52 col 6 */
        for (int f = 52; f <= 56; f++)
            for (int c = 3; c <= 7; c++)
                mapa[f][c] = MURO;
        mapa[52][6] = CAMINO;  /* entrada */
        for (int f = 53; f <= 56; f++)
            for (int c = 4; c <= 6; c++)
                mapa[f][c] = CAMINO;
        mapa[55][4] = LLAVE;
 
        for (int f = 2; f <= 7; f++)
            for (int c = 53; c <= 58; c++)
                mapa[f][c] = MURO;
        /* Interior */
        for (int f = 3; f <= 6; f++)
            for (int c = 54; c <= 57; c++)
                mapa[f][c] = CAMINO;
        mapa[3][54] = SALIDA;
        /* Puerta: único hueco en muro sur (fila 7) */
        mapa[7][55] = PUERTA;
        /* Corredor de llegada horizontal en fila 8 desde apertura anillo3 */
        for (int c = 35; c <= 55; c++) mapa[8][c] = CAMINO;
 
    } /* fin nivel 1 */
 
    /* NIVEL 2 */
    else if (nivel == 2) {
 
        /* ── Muros divisorios ────────────────────────────────────────────── */
        for (int f = 1; f <= 58; f++) {
            mapa[f][15] = MURO;
            mapa[f][30] = MURO;
            mapa[f][45] = MURO;
        }
        for (int c = 1; c <= 58; c++) {
            mapa[15][c] = MURO;
            mapa[30][c] = MURO;
            mapa[45][c] = MURO;
        }
 
        /* ── Aperturas entre salas ───────────────────────────────────────── */
        mapa[7][15]  = CAMINO; /* (0,0)↔(0,1) */
        mapa[7][30]  = CAMINO; /* (0,1)↔(0,2) */
        /* (0,2)↔(0,3): CERRADO — bóveda sólo accesible desde D */
 
        mapa[22][15] = CAMINO; /* (1,0)↔(1,1) */
        mapa[22][30] = CAMINO; /* (1,1)↔(1,2) */
        mapa[22][45] = CAMINO; /* (1,2)↔(1,3) */
 
        mapa[37][15] = CAMINO; /* (2,0)↔(2,1) */
        mapa[37][30] = CAMINO; /* (2,1)↔(2,2) */
        /* (2,2)↔(2,3): CERRADO */
 
        mapa[52][15] = CAMINO; /* (3,0)↔(3,1) */
        mapa[52][30] = CAMINO; /* (3,1)↔(3,2) */
        /* (3,2)↔(3,3): CERRADO — callejón sin salida real */
 
        /* Verticales */
        mapa[15][7]  = CAMINO; /* (0,0)↕(1,0) */
        mapa[30][7]  = CAMINO; /* (1,0)↕(2,0) */
        mapa[45][7]  = CAMINO; /* (2,0)↕(3,0) K */
 
        mapa[15][22] = CAMINO; /* (0,1)↕(1,1) */
        mapa[30][22] = CAMINO; /* (1,1)↕(2,1) */
        mapa[45][22] = CAMINO; /* (2,1)↕(3,1) */
 
        mapa[15][37] = CAMINO; /* (0,2)↕(1,2) */
        mapa[29][37] = CAMINO; /* (1,2)↕(2,2) — hueco justo sobre muro[30] */
        mapa[44][37] = CAMINO; /* (2,2)↕(3,2) */
 
        mapa[30][52] = CAMINO; /* (1,3)↕(2,3) — baja al callejón */
 
        for (int f = 16; f <= 29; f++) mapa[f][52] = CAMINO;
 
        for (int f = 1; f <= 14; f++)
            for (int c = 46; c <= 58; c++)
                mapa[f][c] = MURO;
        /* Interior habitable */
        for (int f = 2; f <= 13; f++)
            for (int c = 47; c <= 57; c++)
                mapa[f][c] = CAMINO;
        mapa[3][57] = SALIDA;
        /* Puerta en el muro que divide (0,3) de (1,3), fila 15 col 52 */
        mapa[15][52] = PUERTA;
        /* Celda de tránsito dentro de la bóveda justo encima de D */
        mapa[14][52] = CAMINO;
 
        /* ── Obstáculos internos en salas ──────────────────────────────── */
        /* Sala (0,1) */
        mapa[4][19]  = MURO; mapa[5][19]  = MURO; mapa[6][19]  = MURO;
        mapa[4][25]  = MURO; mapa[5][25]  = MURO; mapa[6][25]  = MURO;
        mapa[9][20]  = MURO; mapa[10][20] = MURO; mapa[11][20] = MURO;
        mapa[9][26]  = MURO; mapa[10][26] = MURO;
 
        /* Sala (1,1) */
        for (int c = 17; c <= 24; c++) mapa[19][c] = MURO;
        mapa[19][20] = CAMINO; mapa[19][23] = CAMINO;
        for (int c = 17; c <= 28; c++) mapa[24][c] = MURO;
        mapa[24][26] = CAMINO;
 
        /* Sala (2,2) */
        for (int c = 33; c <= 42; c++) mapa[33][c] = MURO;
        mapa[33][38] = CAMINO;
        for (int f = 33; f <= 41; f++) mapa[f][42] = MURO;
        mapa[36][42] = CAMINO;
        for (int c = 35; c <= 42; c++) mapa[40][c] = MURO;
        mapa[40][35] = CAMINO;
 
        /* Sala (1,0) */
        mapa[18][4]  = MURO; mapa[18][5]  = MURO; mapa[18][6]  = MURO;
        mapa[18][8]  = MURO; mapa[18][9]  = MURO; mapa[18][10] = MURO;
        mapa[23][4]  = MURO; mapa[23][5]  = MURO; mapa[23][6]  = MURO;
 
        /* ── Cámara LLAVE K – sala (3,0) ─────────────────────────────────
         * Cubículo interior sellado, entrada única por (48,8).             */
        for (int c = 3; c <= 13; c++) mapa[48][c] = MURO;
        mapa[48][8] = CAMINO; /* única entrada */
        for (int f = 49; f <= 56; f++)
            for (int c = 3; c <= 13; c++)
                mapa[f][c] = CAMINO;
        for (int c = 3; c <= 13; c++) mapa[57][c] = MURO;
        mapa[57][7] = CAMINO;
        mapa[54][4] = LLAVE;
 
    } /* fin nivel 2 */
 
    /* NIVEL 3 */
    else {
 
        /* Paso 1: todo MURO dentro del borde */
        for (int f = 1; f <= 58; f++)
            for (int c = 1; c <= 58; c++)
                mapa[f][c] = MURO;
 
        /* == Cámara de inicio (1..6 , 1..6) == */
        for (int f = 1; f <= 6; f++)
            for (int c = 1; c <= 6; c++)
                mapa[f][c] = CAMINO;
 
        /* == Pasillo al este hasta bifurcación == */
        for (int c = 7; c <= 15; c++) mapa[3][c] = CAMINO;
        for (int f = 3; f <= 14; f++) mapa[f][15] = CAMINO;
 
        /* == Bifurcación A (10..14 , 13..18) == */
        for (int f = 10; f <= 14; f++)
            for (int c = 13; c <= 18; c++)
                mapa[f][c] = CAMINO;
 
        /* ─── RUTA NORTE: callejón sin salida ─────────────────────────── */
        for (int f = 2; f <= 10; f++)  mapa[f][13] = CAMINO;
        for (int c = 13; c <= 22; c++) mapa[2][c]  = CAMINO;
        for (int f = 2; f <= 5; f++)
            for (int c = 20; c <= 27; c++)
                mapa[f][c] = CAMINO;
        for (int c = 27; c <= 35; c++) mapa[3][c]  = CAMINO;
        for (int f = 3; f <= 9; f++)   mapa[f][35] = CAMINO;
 
        /* ─── RUTA ESTE: lleva a K ──────────────────────────────────────── */
        for (int c = 18; c <= 28; c++) mapa[12][c] = CAMINO;
        /* Sala E1 (10..16 , 26..32) */
        for (int f = 10; f <= 16; f++)
            for (int c = 26; c <= 32; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón norte desde E1 (trampa) */
        for (int f = 4; f <= 10; f++)  mapa[f][29] = CAMINO;
        for (int c = 27; c <= 35; c++) mapa[4][c]  = CAMINO;
        /* Continúa este */
        for (int c = 32; c <= 42; c++) mapa[13][c] = CAMINO;
        for (int f = 13; f <= 24; f++) mapa[f][42] = CAMINO;
        /* Sala E2 (22..28 , 40..47) */
        for (int f = 22; f <= 28; f++)
            for (int c = 40; c <= 47; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón oeste desde E2 (trampa) */
        for (int c = 30; c <= 40; c++) mapa[25][c] = CAMINO;
        for (int f = 25; f <= 32; f++) mapa[f][30] = CAMINO;
        /* Continúa sur-este */
        for (int f = 28; f <= 38; f++) mapa[f][46] = CAMINO;
        for (int c = 46; c <= 55; c++) mapa[38][c] = CAMINO;
        /* Sala E3 (36..42 , 53..58) */
        for (int f = 36; f <= 42; f++)
            for (int c = 53; c <= 58; c++)
                mapa[f][c] = CAMINO;
        /* Corredor sur hacia K */
        for (int f = 42; f <= 54; f++) mapa[f][55] = CAMINO;
        /* Cámara K (52..57 , 52..57) */
        for (int f = 52; f <= 57; f++)
            for (int c = 52; c <= 57; c++)
                mapa[f][c] = CAMINO;
        /* Muro interno de la cámara K */
        for (int c = 52; c <= 57; c++) mapa[53][c] = MURO;
        mapa[53][55] = CAMINO;
        mapa[56][52] = LLAVE;
 
        /* ─── RUTA SUR: lleva a D/S ─────────────────────────────────────── */
        for (int f = 14; f <= 26; f++) mapa[f][15] = CAMINO;
        /* Sala S1 (24..30 , 13..21) */
        for (int f = 24; f <= 30; f++)
            for (int c = 13; c <= 21; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón oeste (trampa) */
        for (int c = 3; c <= 13; c++)  mapa[27][c] = CAMINO;
        for (int f = 27; f <= 35; f++) mapa[f][3]  = CAMINO;
        for (int c = 3; c <= 9; c++)   mapa[35][c] = CAMINO;
        /* Continúa sur */
        for (int f = 30; f <= 42; f++) mapa[f][17] = CAMINO;
        /* Sala S2 (40..48 , 15..25) */
        for (int f = 40; f <= 48; f++)
            for (int c = 15; c <= 25; c++)
                mapa[f][c] = CAMINO;
        /* Sub-callejón este desde S2 (trampa) — se detiene en col 34 */
        for (int c = 25; c <= 34; c++) mapa[43][c] = CAMINO;
        for (int f = 43; f <= 50; f++) mapa[f][34] = CAMINO;
        /* Continúa hacia D: corredor que llega justo a (51,46) */
        for (int f = 40; f <= 51; f++) mapa[f][20] = CAMINO;
        for (int c = 20; c <= 46; c++) mapa[51][c] = CAMINO;

        for (int f = 42; f <= 49; f++)
            for (int c = 42; c <= 49; c++)
                mapa[f][c] = CAMINO;
        /* Salida en el rincón NE */
        mapa[42][49] = SALIDA;
        /* Puerta: único hueco en el muro sur de la bóveda (fila 50, col 46) */
        mapa[50][46] = PUERTA;

        for (int c = 20; c <= 30; c++) mapa[34][c] = CAMINO;
        for (int f = 28; f <= 34; f++) mapa[f][20] = CAMINO;
 
        /* Corredor decorativo norte-centro */
        for (int c = 18; c <= 26; c++) mapa[18][c] = CAMINO;
        for (int f = 15; f <= 22; f++) mapa[f][26] = CAMINO;
 
    } /* fin nivel 3 */
 
    /* ── Posición inicial del jugador ────────────────────────────────────── */
    mapa[1][1] = JUGADOR;
}
 

void dibujar_mapa(char m[FILAS][COLUMNAS], int cam_fila, int cam_col) {
    if (cam_fila < 0)                        cam_fila = 0;
    if (cam_col  < 0)                        cam_col  = 0;
    if (cam_fila > FILAS    - CAM_FILAS)     cam_fila = FILAS    - CAM_FILAS;
    if (cam_col  > COLUMNAS - CAM_COLUMNAS)  cam_col  = COLUMNAS - CAM_COLUMNAS;

    for (int fila = cam_fila; fila < cam_fila + CAM_FILAS; fila++) {
        for (int col = cam_col; col < cam_col + CAM_COLUMNAS; col++) {
            putchar(m[fila][col]);
        }
        putchar('\n');
    }
}