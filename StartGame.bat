@echo off
chcp 65001 > nul
title Robot Delivery Simulator - Miami

echo ========================================
echo    ROBOT DELIVERY SIMULATOR
echo ========================================
echo.
echo Iniciando simulador...
echo.

:: Agregar DLLs al PATH
set PATH=%~dp0libraries;%PATH%

:: Ejecutar el juego
bin\RobotDeliverySimulator.exe

:: Mensaje si hay error
if errorlevel 1 (
    echo.
    echo ERROR: No se pudo ejecutar el simulador
    echo.
    echo Soluciones:
    echo 1. Verifica que todos los archivos esten presentes
    echo 2. Instala Visual C++ Redistributable
    echo 3. Ejecuta como Administrador
    echo.
    pause
)
