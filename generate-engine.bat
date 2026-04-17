@echo off

setlocal

echo === Begun: Generating engine project ===
echo.

call Tools/Setup/Engine/generate-engine-cmake.bat
echo.
call Tools/Setup/Engine/copy-engine-dlls.bat

echo === Begun: Generating slnx file ===
cd Tools/Setup

set CUR_DIR=%~dp0
MoveSLN.exe ENGINE %CUR_DIR%/Temp/Binary/Eclipsed-Engine.slnx ../..
echo === Completed: Completed generating slnx file ===

echo.
echo === Completed: Generating engine project ===

endlocal 

pause