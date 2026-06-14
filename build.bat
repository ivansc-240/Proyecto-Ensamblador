@echo off
echo =======================================
echo Iniciando proceso de construccion (Build)
echo =======================================

echo [1/2] Ensamblando rutinas de bajo nivel (NASM 64-bits)...
nasm -f win64 rutinas.asm -o rutinas.o

if %errorlevel% neq 0 (
    echo.
    echo [ERROR CRITICO] Fallo de sintaxis en rutinas.asm. Abortando.
    pause
    exit /b
)

echo [2/2] Compilando modulos C y enlazando sistema (GCC)...
gcc main.c mapa.c jugador.c rutinas.o -o BitQuest.exe

if %errorlevel% equ 0 (
    echo.
    echo =======================================
    echo Construccion exitosa. Ejecutando BitQuest...
    echo =======================================
    BitQuest.exe
) else (
    echo.
    echo =======================================
    echo [ERROR CRITICO] Fallo en el enlazador (Linker) de GCC.
    echo =======================================
)
pause