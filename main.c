#include <stdio.h>
#include "mapa.h"
#include "jugador.h"

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