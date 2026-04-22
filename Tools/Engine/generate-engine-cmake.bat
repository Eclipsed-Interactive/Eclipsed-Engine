@echo off

setlocal

call CheckVSVersion.bat
echo Generator: %VS_FULL_CMAKE%
echo.

cmake -G "%VS_FULL_CMAKE%"                 ^
    -T host=x64  ^
    -S ../Eclipse-Source/  ^
    -B ../Eclipse-Source/Library/Binary  ^
    -DECLIPSED_EDITOR=ON  ^
    -DPROJECT_DIR="%PROJECT_DIR%" ^
    -DINCLUDE_GAME=OFF   ^
    -DBUILD_GAME_PATH="${CMAKE_SOURCE_DIR}/Bin"

endlocal