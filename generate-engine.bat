@echo off

setlocal

echo === Begun: Generating engine project ===
echo.

call Tools/Setup/generate-engine-cmake.bat
echo.
call Tools/Setup/copy-engine-dlls.bat

echo === Begun: Generating slnx file ===
cd Tools/Setup

set CUR_DIR=%~dp0
MoveSLN.exe %CUR_DIR%/Temp/Binary/ ../.. Eclipsed-Engine
echo === Completed: Completed generating slnx file ===

echo.
echo === Completed: Generating engine project ===

endlocal 

pause