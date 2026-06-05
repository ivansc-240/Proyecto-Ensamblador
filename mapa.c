#include <stdio.h>
#include <string.h>
#include "mapa.h"

char mapa[FILAS][COLUMNAS];

void inicializar_mapa(void) {
    for (int f = 0; f < FILAS; f++)
        for (int c = 0; c < COLUMNAS; c++)
            mapa[f][c] = CAMINO;

    for (int f = 0; f < FILAS; f++) {
        mapa[f][0]          = MURO;
        mapa[f][COLUMNAS-1] = MURO;
    }
    for (int c = 0; c < COLUMNAS; c++) {
        mapa[0][c]       = MURO;
        mapa[FILAS-1][c] = MURO;
    }

    for (int c = 1; c < 55; c++) mapa[10][c] = MURO;
    for (int f = 10; f < 35; f++) mapa[f][15] = MURO;
    for (int c = 15; c < 58; c++) mapa[20][c] = MURO;
    for (int f = 20; f < 45; f++) mapa[f][50] = MURO;
    for (int c = 1; c < 50; c++) mapa[35][c] = MURO;
    for (int f = 35; f < 55; f++) mapa[f][8] = MURO;
    for (int c = 8; c < 58; c++) mapa[48][c] = MURO;

    mapa[10][45] = CAMINO;
    mapa[20][15] = CAMINO;
    mapa[20][40] = CAMINO;
    mapa[35][50] = CAMINO;
    mapa[35][10] = CAMINO;
    mapa[48][8] = CAMINO;
    mapa[48][30] = CAMINO;

    /* Muro horizontal que fuerza pasar por la puerta */
    for (int c = 1; c < 59; c++) mapa[55][c] = MURO;
    mapa[55][30] = PUERTA;

    /* Pasillo de una celda hacia la salida */
    mapa[56][29] = MURO;
    mapa[56][31] = MURO;
    mapa[57][29] = MURO;
    mapa[57][31] = MURO;
    mapa[57][30] = SALIDA;

    /* Jugador y llave */
    mapa[1][1]  = JUGADOR;
    mapa[8][55] = LLAVE;
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