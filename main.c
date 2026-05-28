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

/* Posicion del jugador */
int jugador_fila = 1;
int jugador_col  = 1;

/* Dibuja el mapa en consola */
void dibujar_mapa(char m[FILAS][COLUMNAS]) {
    for (int fila = 0; fila < FILAS; fila++) {
        for (int col = 0; col < COLUMNAS; col++) {
            putchar(m[fila][col]);
        }
        putchar('\n');
    }
}

void mover_jugador(char tecla) {
    int nueva_fila = jugador_fila;
    int nueva_col  = jugador_col;

    if (tecla == 'w') nueva_fila--;
    if (tecla == 's') nueva_fila++;
    if (tecla == 'a') nueva_col--;
    if (tecla == 'd') nueva_col++;

    mapa[jugador_fila][jugador_col] = CAMINO;
    jugador_fila = nueva_fila;
    jugador_col  = nueva_col;
    mapa[jugador_fila][jugador_col] = JUGADOR;
}

int main(void) {
    char tecla;

    while (1) {
        dibujar_mapa(mapa);

        printf("WASD para mover, Q para salir: ");
        tecla = getchar();

        while (getchar() != '\n');

        if (tecla == 'q') break;

        mover_jugador(tecla);
    }

    return 0;
}