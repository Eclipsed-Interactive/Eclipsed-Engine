@echo off

title Engine Generation

setlocal

echo Saving engine root as env variable: 

setx ECLIPSED_ENGINE_ROOT "%~dp0\"

echo.
echo.

echo ==== Begun: Generating project ====
echo.

cmake -G "Visual Studio 18 2026" ^
    -T host=x64 ^
    -S . ^
    -B Temp/Binary ^
    -DBUILD_EDITOR=ON


echo.
call Tools/Setup/copy-engine-dlls.bat
call Tools/Setup/copy-etc-files.bat

echo === Begun: Generating slnx file ===
cd Tools/Setup

set CUR_DIR=%~dp0
MoveSLN.exe %CUR_DIR%/Temp/Binary/ ../.. Eclipsed-Engine
echo === Completed: Completed generating slnx file ===

echo.
echo === Completed: Generating engine project ===

endlocal 

pause