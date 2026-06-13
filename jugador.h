#ifndef JUGADOR_H
#define JUGADOR_H

extern int jugador_fila;
extern int jugador_col;
extern int jugador_tiene_llave;
extern int jugador_gano;
extern int jugador_monedas;
extern int monedas_nivel;

void mover_jugador(char tecla);
int  es_muro_asm(char celda);

#endif