@echo off

setlocal

set /p PROJECT_PATH=< "../Eclipse-Source/Bin/.ini"

call CheckVSVersion.bat
echo Generator: %VS_FULL_CMAKE%
echo.

cmake -G "%VS_FULL_CMAKE%"                 ^
    -T host=x64  ^
    -S ../Eclipse-Source/  ^
    -B "%PROJECT_PATH%/Library/Game-Binary"  ^
    -DECLIPSED_EDITOR=OFF  ^
    -DINCLUDE_GAME=ON   ^
    -DBUILD_GAME_PATH="%PROJECT_PATH%/Bin"

endlocal