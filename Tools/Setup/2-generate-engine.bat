@echo off

setlocal

echo === Begun: Generating engine project ===
echo.

call Engine/generate-engine-cmake.bat
echo.
call Engine/copy-engine-dlls.bat

echo === Begun: Generating slnx file ===
cd /d "%~dp0"
MoveSLN.exe ENGINE ../Eclipse-Source/Library/Binary/Eclipsed-Engine.slnx ..
echo === Completed: Completed generating slnx file ===

echo.
echo === Completed: Generating engine project ===

endlocal 

pause