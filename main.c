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

    char tecla;
    int nivel_actual = 1;
    inicializar_mapa(nivel_actual); 

    printf("¡Bienvenido a BitQuest!\n");
    printf("¿Listo para jugar? Presiona cualquier tecla...\n");
    _getch();

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    while (1) {
        int cam_fila = jugador_fila - CAM_FILAS    / 2;
        int cam_col  = jugador_col  - CAM_COLUMNAS / 2;

        system("cls");
        dibujar_mapa(mapa, cam_fila, cam_col);

        if (jugador_gano) {
            if (nivel_actual < 3) {
                nivel_actual++;
                inicializar_mapa(nivel_actual);
                jugador_fila        = 1;
                jugador_col         = 1;
                jugador_tiene_llave = 0;
                jugador_gano        = 0;
                system("cls");
                printf("¡Nivel %d!\n", nivel_actual);
                printf("Presiona cualquier tecla para continuar...\n");
                _getch();
                continue;
            } else {
                system("cls");
                printf("¡Felicidades! Completaste los 3 niveles de BitQuest.\n");
                printf("Presiona cualquier tecla para salir...\n");
                _getch();
                break;
            }
        }

        printf("Nivel: %d/3 | Llave: %s | WASD para mover, Q para salir\n",
               nivel_actual, jugador_tiene_llave ? "SÍ" : "NO");

        tecla = _getch();

        if (tecla == 'q') break;

        mover_jugador(tecla);
    }

    return 0;
}