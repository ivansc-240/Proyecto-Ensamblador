#include "mapa.h"
#include "jugador.h"

int jugador_fila = 1;
int jugador_col  = 1;
int jugador_tiene_llave = 0;
int jugador_gano       = 0;
int jugador_monedas    = 0;// monedas recogidas en el nivel actual
int monedas_nivel      = 1;// cuántas se requieren (lo fija main.c)  