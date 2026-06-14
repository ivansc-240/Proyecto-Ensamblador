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
    int pasos_nivel      = 0;   // pasos en el nivel actual (se reinicia al cambiar nivel)
    int pasos_totales    = 0;   // pasos acumulados en toda la partida
    time_t tiempo_inicio = time(NULL);

    jugador_niveles = 0;            
    jugador_pasos   = 0;            

    //Pantalla de Inicio
    system("cls");
    printf("\n\n");
    printf("  ██████╗ ██╗████████╗ ██████╗ ██╗   ██╗███████╗███████╗████████╗\n");
    printf("  ██╔══██╗██║╚══██╔══╝██╔═══██╗██║   ██║██╔════╝██╔════╝╚══██╔══╝\n");
    printf("  ██████╔╝██║   ██║   ██║   ██║██║   ██║█████╗  ███████╗   ██║   \n");
    printf("  ██╔══██╗██║   ██║   ██║▄▄ ██║██║   ██║██╔══╝  ╚════██║   ██║   \n");
    printf("  ██████╔╝██║   ██║   ╚██████╔╝╚██████╔╝███████╗███████║   ██║   \n");
    printf("  ╚═════╝ ╚═╝   ╚═╝    ╚══▀▀═╝  ╚═════╝ ╚══════╝╚══════╝   ╚═╝   \n");
    printf("\n");
    printf("            I N I C I A L I Z A N D O   E N T O R N O\n");
    printf("\n\n\n");
    printf("        > Presione cualquier tecla para iniciar partida <\n");
    _getch();

    inicializar_mapa(nivel_actual);
    monedas_nivel = contar_caracter_mapa((char *)mapa, FILAS * COLUMNAS, '$');

    int celdas_libres = contar_celdas_libres((char *)mapa, FILAS * COLUMNAS);
    
    system("cls");
    printf("¡Nivel %d!\n", nivel_actual);
    printf("Diagnostico de memoria: %d celdas libres detectadas.\n", celdas_libres);
    printf("Comenzando secuencia... Presiona cualquier tecla.\n");
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
            monedas_total   += jugador_monedas;
            llaves_total    += 1;
            pasos_totales   += pasos_nivel;     // sumar pasos del nivel al total
            jugador_niveles += 1;               // para calcular_puntaje() (Función 3)

            if (nivel_actual < 3) {
                // Pantalla de resumen del nivel
                system("cls");
                printf("=================================\n");
                printf("  Nivel %d completado\n", nivel_actual);
                printf("  Monedas recolectadas: %d/%d\n", jugador_monedas, monedas_nivel);
                printf("  Pasos realizados: %d\n", pasos_nivel);
                printf("=================================\n\n");
                printf("Presiona cualquier tecla para continuar...\n");
                _getch();

                nivel_actual++;
                pasos_nivel         = 0;        // reiniciar contador de pasos del nivel
                jugador_monedas     = 0;        // reiniciar monedas del nivel
                inicializar_mapa(nivel_actual);
                
                // contar monedas reales del nuevo mapa
                monedas_nivel = contar_caracter_mapa((char *)mapa, FILAS * COLUMNAS, '$');
                
                int celdas_libres = contar_celdas_libres((char *)mapa, FILAS * COLUMNAS);
                
                jugador_fila        = 1;
                jugador_col         = 1;
                jugador_tiene_llave = 0;
                jugador_gano        = 0;

                system("cls");
                printf("¡Nivel %d!\n", nivel_actual);
                printf("Diagnostico de memoria: %d celdas libres detectadas.\n", celdas_libres);
                printf("Comenzando secuencia... Presiona cualquier tecla.\n");
                _getch();
                continue;
            } else {
                // Pantalla de resumen final
                int segundos = (int)difftime(time(NULL), tiempo_inicio);
                int mm       = segundos / 60;
                int ss       = segundos % 60;

                // Puntaje final: incluir el último nivel completado
                pasos_totales += pasos_nivel;
                int puntaje_final = calcular_puntaje(monedas_total,
                                                     pasos_totales,
                                                     jugador_niveles);
                system("cls");
                printf("╔══════════════════════════════════════╗\n");
                printf("║       BitQuest  -  Completado        ║\n");
                printf("╠══════════════════════════════════════╣\n");
                printf("║  Niveles completados :  3 / 3        ║\n");
                printf("║  Monedas recogidas   :  %d / 6        ║\n", monedas_total);
                printf("║  Llaves recogidas    :  %d / 3        ║\n", llaves_total);
                printf("║  Pasos totales       :  %-4d         ║\n", pasos_totales);
                printf("║  Puntaje final       :  %-4d         ║\n", puntaje_final);
                printf("║  Tiempo total        :  %02d:%02d        ║\n", mm, ss);
                printf("╚══════════════════════════════════════╝\n");
                printf("\n  Presiona cualquier tecla para salir...\n");
                _getch();
                break;
            }
        }

        printf("Nivel: %d/3 | Llave: %s | Monedas: %d/%d | Pasos: %d | WASD mover, Q salir\n",
               nivel_actual,
               jugador_tiene_llave ? "SÍ" : "NO",
               jugador_monedas,
               monedas_nivel,
               pasos_nivel);

        tecla = _getch();

        if (tecla == 'q') break;

        // Incrementar pasos solo ante teclas de movimiento válidas
        if (tecla == 'w' || tecla == 'a' || tecla == 's' || tecla == 'd') {
            pasos_nivel++;
            jugador_pasos++;           // sincronizar con la variable global de ASM
        }

        mover_jugador(tecla);
    }

    return 0;
}