@echo off
echo Compilando modulo NASM de 64 bits...
nasm -f win64 jugador.asm -o jugador_asm.o

if %errorlevel% neq 0 (
    echo [ERROR] Fallo al compilar jugador.asm
    pause
    exit /b
)

echo Enlazando modulos con GCC...
gcc main.c mapa.c jugador.c jugador_asm.o -o bitquest.exe

if %errorlevel% equ 0 (
    echo ---------------------------------------
    echo Ejecutando BitQuest:
    echo ---------------------------------------
    bitquest.exe
) else (
    echo ---------------------------------------
    echo [ERROR] Hubo un fallo al enlazar el proyecto.
    echo ---------------------------------------
)
pause