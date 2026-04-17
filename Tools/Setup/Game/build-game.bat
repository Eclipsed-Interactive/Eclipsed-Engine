@echo off

setlocal

set /p PROJECT_PATH=< "../Eclipse-Source/Bin/.ini"

cmake --build "%PROJECT_PATH%/Library/Game-Binary" --config %1

endlocal
