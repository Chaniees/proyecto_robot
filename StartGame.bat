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
set PATH=%~dp0bin;%~dp0libraries;%PATH%

:: Ejecutar el juego directamente
if exist "bin\RobotDeliverySimulator.exe" (
    echo Ejecutando juego...
    bin\RobotDeliverySimulator.exe
) else (
    echo ERROR: Ejecutable no encontrado
    echo.
    echo Descarga el proyecto completo desde GitHub
    echo https://github.com/Chaniees/proyecto_robot
    echo.
    pause
)
