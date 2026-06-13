#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>
#include "mapa.h"
#include "jugador.h"

int main(void) {
    setlocale(LC_ALL, "es_MX.UTF-8");
    SetConsoleOutputCP(65001);

    char tecla;
    int nivel_actual     = 1;
    int monedas_total    = 0;   // acumulado entre niveles (1+2+3 = 6)
    int llaves_total     = 0;   // 1 por nivel = 3
    time_t tiempo_inicio = time(NULL);

    monedas_nivel = nivel_actual;
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
            // Acumular estadísticas del nivel recién completado
            monedas_total += jugador_monedas;
            llaves_total  += 1;

            if (nivel_actual < 3) {
                nivel_actual++;
                inicializar_mapa(nivel_actual);
                jugador_fila        = 1;
                jugador_col         = 1;
                jugador_tiene_llave = 0;
                jugador_gano        = 0;
                monedas_nivel       = nivel_actual;  
                system("cls");
                printf("¡Nivel %d!\n", nivel_actual);
                printf("Presiona cualquier tecla para continuar...\n");
                _getch();
                continue;
            } else {
                // Pantalla de resumen final
                int segundos = (int)difftime(time(NULL), tiempo_inicio);
                int mm       = segundos / 60;
                int ss       = segundos % 60;

                system("cls");
                printf("╔══════════════════════════════════════╗\n");
                printf("║       BitQuest  -  Completado        ║\n");
                printf("╠══════════════════════════════════════╣\n");
                printf("║  Niveles completados :  3 / 3        ║\n");
                printf("║  Monedas recogidas   :  %d / 6        ║\n", jugador_monedas);
                printf("║  Llaves recogidas    :  %d / 3        ║\n", llaves_total);
                printf("║  Tiempo total        :  %02d:%02d        ║\n", mm, ss);
                printf("╚══════════════════════════════════════╝\n");
                printf("\n  Presiona cualquier tecla para salir...\n");
                _getch();
                break;
            }
        }

        printf("Nivel: %d/3 | Llave: %s | Monedas: %d | WASD para mover, Q para salir\n",
               nivel_actual,
               jugador_tiene_llave ? "SÍ" : "NO",
               jugador_monedas);

        tecla = _getch();

        if (tecla == 'q') break;

        mover_jugador(tecla);
    }

    return 0;
}