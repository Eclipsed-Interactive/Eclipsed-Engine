@echo off

setlocal

set /p PROJECT_PATH=< "../Eclipse-Source/Bin/.ini"

cmake --build "%PROJECT_PATH%/Library/Game-Binary" --config Release

endlocal
