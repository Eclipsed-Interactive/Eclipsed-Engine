@echo off

setlocal

if "%1" == ""(
    echo No build target position.
    timeout /t 5
    exit /b 1
)

@REM Build binary with game and eclipsed engine as static library
@rem Build the project

@rem Pack the assets

endlocal