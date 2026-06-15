/*
 * Módulo: main.c
 * Operación: Punto de entrada del binario ejecutable. Control del flujo de
 * ejecución principal, manejo del búfer de salida de consola, captura de
 * interrupciones de teclado e integración de rutinas en Ensamblador y C.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <locale.h>
#include <time.h>
#include <windows.h>
#include "mapa.h"
#include "jugador.h"

/*
 * Función: main
 * Entrada: Void
 * Salida: Entero (0 para terminación exitosa del proceso)
 * Operación en memoria/estado: Inicializa la paginación de memoria de la 
 * consola, reserva variables de estado de la sesión en la pila, establece el
 * bucle de ejecución principal (while infinito) y gestiona los saltos 
 * condicionales de transición de nivel y terminación del proceso.
 */
int main(void) {
    /* Modificación del entorno de ejecución de la consola para soporte UTF-8 */
    setlocale(LC_ALL, "es_MX.UTF-8");
    SetConsoleOutputCP(65001);

    char tecla;
    int nivel_actual     = 1;
    int monedas_total    = 0;
    int llaves_total     = 0;
    int pasos_nivel      = 0;
    int pasos_totales    = 0;
    int pantalla_sucia   = 1;
    time_t tiempo_inicio = time(NULL);

    /* Reinicio de registros de estado exportados hacia rutinas en Ensamblador */
    jugador_niveles = 0;            
    jugador_pasos   = 0;            

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
    
    /* * Cast de la matriz bidimensional 'mapa' a un puntero plano unidimensional
     * (char *) para el cálculo secuencial de bytes en el bloque de memoria de tamaño FILAS * COLUMNAS.
     */
    monedas_nivel = contar_caracter_mapa((char *)mapa, FILAS * COLUMNAS, MONEDA);

    int celdas_libres = contar_celdas_libres((char *)mapa, FILAS * COLUMNAS);
    
    system("cls");
    printf("¡Nivel %d!\n", nivel_actual);
    printf("Diagnostico de memoria: %d celdas libres detectadas.\n", celdas_libres);
    printf("Comenzando secuencia... Presiona cualquier tecla.\n");
    _getch();

    /* * Obtención del descriptor de archivo estándar de salida de Windows.
     * Modificación de la máscara de bits (DWORD mode) para habilitar el 
     * procesamiento del búfer en modo de terminal virtual (secuencias ANSI).
     */
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hOut, &mode);
    SetConsoleMode(hOut, mode | ENABLE_PROCESSED_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    while (1) {
        /* Cálculo del desplazamiento aritmético de la matriz virtual de la cámara */
        int cam_fila = jugador_fila - CAM_FILAS    / 2;
        int cam_col  = jugador_col  - CAM_COLUMNAS / 2;

        /* Alternancia de métodos de limpieza del búfer de salida gráfico */
        if (pantalla_sucia) {
            system("cls");
            pantalla_sucia = 0;
        } else {
            /* Retorno de carro y salto al inicio del búfer mediante escape ANSI */
            printf("\033[H");
        }
        dibujar_mapa(mapa, cam_fila, cam_col);

        if (jugador_gano) {
            monedas_total   += jugador_monedas;
            llaves_total    += 1;
            pasos_totales   += pasos_nivel;
            jugador_niveles += 1;

            if (nivel_actual < 4) {
                system("cls");
                printf("=================================\n");
                printf("  Nivel %d completado\n", nivel_actual);
                printf("  Monedas recolectadas: %d/%d\n", jugador_monedas, monedas_nivel);
                printf("  Pasos realizados: %d\n", pasos_nivel);
                printf("=================================\n\n");
                printf("Presiona cualquier tecla para continuar...\n");
                _getch();

                nivel_actual++;
                pasos_nivel         = 0;
                jugador_monedas     = 0;
                inicializar_mapa(nivel_actual);
                
                monedas_nivel = contar_caracter_mapa((char *)mapa, FILAS * COLUMNAS, MONEDA);
                int celdas_libres = contar_celdas_libres((char *)mapa, FILAS * COLUMNAS);
                
                jugador_fila        = 1;
                jugador_col         = 1;
                jugador_tiene_llave = 0;
                jugador_gano        = 0;
                pantalla_sucia      = 1;

                system("cls");
                printf("¡Nivel %d!\n", nivel_actual);
                printf("Diagnostico de memoria: %d celdas libres detectadas.\n", celdas_libres);
                printf("Comenzando secuencia... Presiona cualquier tecla.\n");
                _getch();
                continue;
            } else {
                int segundos = (int)difftime(time(NULL), tiempo_inicio);
                int mm       = segundos / 60;
                int ss       = segundos % 60;

                pasos_totales += pasos_nivel;
                
                /* Invocación de subrutina con parámetros pasados por valor */
                int puntaje_final = calcular_puntaje(monedas_total,
                                                     pasos_totales,
                                                     jugador_niveles);
                system("cls");
                printf("╔══════════════════════════════════════╗\n");
                printf("║       BitQuest  -  Completado        ║\n");
                printf("╠══════════════════════════════════════╣\n");
                printf("║  Niveles completados :  4 / 4        ║\n");
                printf("║  Monedas recogidas   :  %d / 20       ║\n", monedas_total);
                printf("║  Llaves recogidas    :  %d / 4        ║\n", llaves_total);
                printf("║  Pasos totales       :  %-4d         ║\n", pasos_totales);
                printf("║  Puntaje final       :  %-4d         ║\n", puntaje_final);
                printf("║  Tiempo total        :  %02d:%02d        ║\n", mm, ss);
                printf("╚══════════════════════════════════════╝\n");
                printf("\n  Presiona cualquier tecla para salir...\n");
                _getch();
                break;
            }
        }

        printf("Nivel: %d/4 | Llave: %s | Monedas: %d/%d | Pasos: %d | WASD mover, Q salir\n",
               nivel_actual,
               jugador_tiene_llave ? "SÍ" : "NO",
               jugador_monedas,
               monedas_nivel,
               pasos_nivel);

        /* Interrupción sincrónica: el proceso bloquea la ejecución en espera de I/O de hardware */
        tecla = _getch();

        if (tecla == 'q') break;

        if (tecla == 'w' || tecla == 'a' || tecla == 's' || tecla == 'd') {
            pasos_nivel++;
            jugador_pasos++;
        }

        mover_jugador(tecla);
    }

    return 0;
}