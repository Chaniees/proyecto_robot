@echo off
chcp 65001 > nul
title Robot Delivery Simulator - Miami

echo ========================================
echo    ROBOT DELIVERY SIMULATOR
echo ========================================
echo.
echo Compilando juego...
echo.

:: Compilar el código
g++ luffy.cpp -o robotg.exe -I libraries -L libraries

:: Ejecutar el juego
robotg.exe

:: Mensaje si hay error
if errorlevel 1 (
    echo.
    echo ERROR: No se pudo ejecutar el simulador
    echo.
    echo Soluciones:
    echo 1. Instalar MinGW-w64 para compilar
    echo 2. Ejecutar como Administrador
    echo.
    pause
)