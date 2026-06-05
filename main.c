#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <windows.h>
#include "mapa.h"
#include "jugador.h"

int main(void) {
    setlocale(LC_ALL, "es_MX.UTF-8");
    SetConsoleOutputCP(65001);

    inicializar_mapa();
    char tecla;

    printf("¡Bienvenido a BitQuest: Explorador de Matrices!\n");
    printf("¿Listo para jugar? Presiona cualquier tecla...\n");
    _getch();

    while (1) {
        int cam_fila = jugador_fila - CAM_FILAS    / 2;
        int cam_col  = jugador_col  - CAM_COLUMNAS / 2;

        system("cls");
        dibujar_mapa(mapa, cam_fila, cam_col);

        if (jugador_gano) {
            printf("¡Ganaste! Encontraste la salida.\n");
            break;
        }

        printf("Llave: %s | WASD para mover, Q para salir\n",
               jugador_tiene_llave ? "SÍ" : "NO");

        tecla = _getch();

        if (tecla == 'q') break;

        mover_jugador(tecla);
    }

    return 0;
}