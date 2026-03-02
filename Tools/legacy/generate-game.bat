@echo off

if "%*"=="" (
    echo No arguments paused. Cant generate game.
    echo Exiting with code 1.
    timeout /t 5
    exit /b 1
)

cd %1

cmake -G "Visual Studio 17 2022" -T host=x64 -S . -B Library/Engine-Build -DPROJECT_DIR="%1" -DENGINE_PATH="%2"