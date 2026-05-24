#include <stdio.h>

/* Constantes del mapa */
#define FILAS 10
#define COLUMNAS 10

/* Caracteres del juego */
#define MURO    '#'
#define CAMINO  ' '
#define JUGADOR 'P'
#define LLAVE   'K'
#define PUERTA  'D'
#define SALIDA  'S'

/* Mapa 10x10 */
char mapa[FILAS][COLUMNAS] = {
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
    {'#', 'P', ' ', ' ', '#', ' ', ' ', ' ', 'K', '#'},
    {'#', ' ', '#', ' ', '#', ' ', '#', '#', ' ', '#'},
    {'#', ' ', '#', ' ', ' ', ' ', '#', ' ', ' ', '#'},
    {'#', ' ', '#', '#', '#', ' ', '#', ' ', '#', '#'},
    {'#', ' ', ' ', ' ', '#', ' ', ' ', ' ', ' ', '#'},
    {'#', '#', ' ', '#', '#', '#', ' ', '#', ' ', '#'},
    {'#', ' ', ' ', ' ', ' ', ' ', ' ', '#', 'D', '#'},
    {'#', ' ', '#', '#', ' ', '#', ' ', ' ', 'S', '#'},
    {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}
};

/* Dibujar el mapa en consola */
void dibujar_mapa(char m[FILAS][COLUMNAS]) {
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            putchar(m[fila][col]);
        }
        putchar('\n');
    }
}

int main(void) {
    dibujar_mapa(mapa);
    return 0;
}