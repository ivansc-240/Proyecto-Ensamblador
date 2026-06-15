#ifndef MAPA_H
#define MAPA_H

/*
 * Módulo: mapa.h
 * Operación: Definición de parámetros físicos del entorno, simbología de 
 * entidades y firmas de interfaz para el subsistema de memoria gráfica del mapa.
 */

/* Dimensiones absolutas del bloque de memoria de la cuadrícula (60x60 bytes) */
#define FILAS    60
#define COLUMNAS 60

/* Dimensiones relativas del marco de visualización del motor de renderizado */
#define CAM_FILAS    20
#define CAM_COLUMNAS 20

/* * Simbología ASCII de colisiones y entidades.
 * Implementación de estándar unificado en español.
 */
#define MURO    '#'
#define CAMINO  '.'
#define JUGADOR 'P'
#define LLAVE   'K'
#define PUERTA  'D'
#define SALIDA  'E'
#define MONEDA  'M'

/*
 * Estructura de Datos: mapa
 * Operación: Segmento de datos externos. Matriz bidimensional de contigüidad
 * de memoria en fila mayor que almacena el estado persistente del nivel.
 */
extern char mapa[FILAS][COLUMNAS];

/*
 * Función: dibujar_mapa
 * Entrada: m (Puntero base a char[60][60]), cam_fila (Offset Y), cam_col (Offset X)
 * Salida: Void
 * Operación: Calcula desplazamientos relativos sobre la matriz 'm'. Recorre 
 * bloques de memoria de tamaño CAM_FILAS x CAM_COLUMNAS desde el origen 
 * [cam_fila, cam_col] y transfiere los valores a la salida estándar del sistema.
 */
void dibujar_mapa(char m[FILAS][COLUMNAS], int cam_fila, int cam_col);

/*
 * Función: inicializar_mapa
 * Entrada: nivel (Índice entero identificador del mapa objetivo)
 * Salida: Void
 * Operación: Realiza carga y escritura de matriz. Asigna caracteres estáticos en
 * la matriz global 'mapa' basada en las rutinas de generación del 'nivel'. 
 * Sobrescribe el buffer de la matriz de forma lineal.
 */
void inicializar_mapa(int nivel);

#endif