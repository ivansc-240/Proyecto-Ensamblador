#include <stdio.h>
#include <stdlib.h>
#include "mapa.h"
#include "jugador.h"

int main(void) {
    char tecla;

    while (1) {
        system("cls");
        dibujar_mapa(mapa);

        if (jugador_gano) {
            printf("¡Ganaste!\n");
            break;
        }

        printf("Llave: %s | WASD para mover, Q para salir\n", jugador_tiene_llave ? "SI" : "NO");
        tecla = getchar();

        while (getchar() != '\n');

        if (tecla == 'q') break;

        mover_jugador(tecla);
    }

    return 0;
}