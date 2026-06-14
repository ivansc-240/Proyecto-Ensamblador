#include "mapa.h"
#include "jugador.h"

int jugador_fila        = 1;
int jugador_col         = 1;
int jugador_tiene_llave = 0;
int jugador_gano        = 0;
int jugador_monedas     = 0;
int monedas_nivel       = 1;
int jugador_pasos       = 0;
int jugador_puntaje     = 0;
int jugador_niveles     = 0;

void mover_jugador(char tecla) {
    int f_nueva = jugador_fila;
    int c_nueva = jugador_col;

    if (tecla == 'w') f_nueva--;
    else if (tecla == 's') f_nueva++;
    else if (tecla == 'a') c_nueva--;
    else if (tecla == 'd') c_nueva++;
    else return;

    if (validar_movimiento((char*)mapa, COLUMNAS, f_nueva, c_nueva) == 0) return;

    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, 'P')) {
        if (!jugador_tiene_llave) return;
    }

    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, 'L')) {
        jugador_tiene_llave = 1;
    }

    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, '$')) {
        jugador_monedas++;
    }

    if (detectar_objeto_celda((char*)mapa, COLUMNAS, f_nueva, c_nueva, 'S')) {
        jugador_gano = 1;
    }

    mapa[jugador_fila][jugador_col] = ' ';
    jugador_fila = f_nueva;
    jugador_col  = c_nueva;
    mapa[jugador_fila][jugador_col] = 'J';
}