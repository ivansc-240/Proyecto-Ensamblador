#include "mapa.h"
#include "jugador.h"

int jugador_fila = 1;
int jugador_col  = 1;

void mover_jugador(char tecla) {
    int nueva_fila = jugador_fila;
    int nueva_col  = jugador_col;

    if (tecla == 'w') nueva_fila--;
    if (tecla == 's') nueva_fila++;
    if (tecla == 'a') nueva_col--;
    if (tecla == 'd') nueva_col++;

    if (nueva_fila < 0 || nueva_fila >= FILAS) return;
    if (nueva_col  < 0 || nueva_col  >= COLUMNAS) return;

    if (mapa[nueva_fila][nueva_col] == MURO) return;

    mapa[jugador_fila][jugador_col] = CAMINO;
    jugador_fila = nueva_fila;
    jugador_col  = nueva_col;
    mapa[jugador_fila][jugador_col] = JUGADOR;
}