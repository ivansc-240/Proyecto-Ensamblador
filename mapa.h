#ifndef MAPA_H
#define MAPA_H

#define FILAS    60
#define COLUMNAS 60

#define CAM_FILAS    20
#define CAM_COLUMNAS 20

#define MURO    '#'
#define CAMINO  ' '
#define JUGADOR 'J'
#define LLAVE   'L'
#define PUERTA  'P'
#define SALIDA  'S'
#define MONEDA  '$'

extern char mapa[FILAS][COLUMNAS];

void dibujar_mapa(char m[FILAS][COLUMNAS], int cam_fila, int cam_col);

void inicializar_mapa(int nivel);

#endif