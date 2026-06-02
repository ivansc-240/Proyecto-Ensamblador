#ifndef MAPA_H
#define MAPA_H

#define FILAS    10
#define COLUMNAS 10

#define MURO    '#'
#define CAMINO  ' '
#define JUGADOR 'P'
#define LLAVE   'K'
#define PUERTA  'D'
#define SALIDA  'S'

extern char mapa[FILAS][COLUMNAS];

void dibujar_mapa(char m[FILAS][COLUMNAS]);

#endif