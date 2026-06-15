\*\*BitQuest\*\* – Entorno interactivo bidimensional modular desarrollado bajo arquitectura híbrida \*\*C + Ensamblador x86-64 NASM\*\*.  

Sistema de laberintos con cámara dinámica, recolección de objetos, puertas lógicas y puntaje basado en eficiencia.



\## Especificaciones del Entorno



| Componente               | Especificación                                |

|--------------------------|-----------------------------------------------|

| \*\*Lenguajes\*\*            | C11, Ensamblador NASM x86-64                  |

| \*\*Arquitectura objetivo\*\*| x86-64 (AMD64)                                |

| \*\*Sistema Operativo\*\*    | Windows 10/11 (64 bits)                       |

| \*\*Compilador C\*\*         | MinGW-w64 (GCC)                               |

| \*\*Ensamblador\*\*          | NASM (Netwide Assembler) 2.16+                |

| \*\*Terminal\*\*             | Compatible con UTF-8 y secuencias ANSI        |



\## Instrucciones de Compilación y Ejecución



\### Método automatizado (recomendado)



Ejecutar el script `build.bat` incluido en la raíz del proyecto:



Este script ejecuta secuencialmente:

&#x09;nasm -f win64 rutinas.asm -o rutinas.o

&#x09;gcc main.c mapa.c jugador.c rutinas.o -o BitQuest.exe

&#x09;Ejecuta BitQuest.exe si el enlazado es exitoso.

\###Compilación manual paso a paso



nasm -f win64 rutinas.asm -o rutinas.o

gcc main.c mapa.c jugador.c rutinas.o -o BitQuest.exe

BitQuest.exe



\##Simbología del Mapa



Ítem	Macro	Carácter	Comportamiento lógico

Muro	MURO	#	Infranqueable. Bloquea movimiento.

Camino	CAMINO	.	Libre tránsito.

Jugador	JUGADOR	P	Entidad controlable. Posición inicial (1,1).

Llave	LLAVE	K	Permite atravesar PUERTA y activar SALIDA.

Puerta	PUERTA	D	Transitable solo si jugador\_tiene\_llave == 1.

Salida	SALIDA	E	Avanza al siguiente nivel. Requiere jugador\_tiene\_llave.

Moneda	MONEDA	M	Incrementa jugador\_monedas. Opcional para métrica final.



\##Interacción Híbrida C + Ensamblador

\###Convención de llamadas: Windows x64 ABI (fastcall)



Registros parámetros	Registro retorno	Shadow space	Preservación

RCX, RDX, R8, R9	RAX	32 bytes	RBX, RSI, RDI, RBP, R12-R15



\###Funciones implementadas en NASM



Función	Prototipo C	Propósito

contar\_caracter\_mapa	int contar\_caracter\_mapa(char \*mapa, int total\_celdas, char car);	Cuenta ocurrencias de un carácter en la matriz linealizada.

validar\_movimiento	int validar\_movimiento(char \*mapa, int columnas, int fila, int col);	Verifica colisión contra muros (#).

calcular\_puntaje	int calcular\_puntaje(int monedas, int pasos, int niveles);	Aplica fórmula: (monedas\*100) + (niveles\*500) - pasos.

detectar\_objeto\_celda	int detectar\_objeto\_celda(char\*, int, int, int, char);	Lee 5.º parámetro desde \[RSP+40]. Compara celda con objeto.

contar\_celdas\_libres	int contar\_celdas\_libres(char \*mapa, int total\_celdas);	Cuenta espacios ' ' en el mapa.

es\_muro\_asm		int es\_muro\_asm(char celda);	Auxiliar de compatibilidad. Retorna 1 si celda == '#'.



Estructura del Proyecto



├── main.c              # Bucle principal, HUD, temporización, transiciones

├── mapa.c              # Inicialización estática de 4 niveles, renderizado

├── mapa.h              # Constantes (60x60, macros de simbología)

├── jugador.c           # Variables globales de estado, wrapper de movimiento

├── jugador.h           # Declaraciones y prototipos

├── rutinas.asm         # Lógica analítica en ensamblador (5 funciones)

├── build.bat           # Script de compilación y ejecución

└── README.md           # Este archivo

