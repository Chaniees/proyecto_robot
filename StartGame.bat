@echo off
chcp 65001 > nul
title Robot Delivery Simulator - Miami

echo ========================================
echo    ROBOT DELIVERY SIMULATOR
echo ========================================
echo.
echo Compilando juego...
echo.

:: Crear carpeta bin si no existe
if not exist "bin" mkdir bin

:: Compilar el c?digo principal con SFML
g++ luffy.cpp -o bin\RobotDeliverySimulator.exe -I libraries -L libraries -lsfml-graphics -lsfml-window -lsfml-system

:: Ejecutar el juego
if exist "bin\RobotDeliverySimulator.exe" (
    echo Ejecutando simulador...
    bin\RobotDeliverySimulator.exe
) else (
    echo ERROR: No se pudo compilar el juego
    echo.
    echo Soluciones:
    echo 1. Instala MinGW-w64 
    echo 2. Verifica que libraries/ tenga los archivos SFML
    echo 3. Ejecuta como Administrador
    echo.
    pause
)
