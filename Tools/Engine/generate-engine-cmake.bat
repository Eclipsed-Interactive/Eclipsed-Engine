@echo off

setlocal

cmake -G "Visual Studio 17 2022"                 ^
    -T host=x64  ^
    -S ../Eclipse-Source/  ^
    -B ../Eclipse-Source/Library/Binary  ^
    -DECLIPSED_EDITOR=ON  ^
    -DPROJECT_DIR="%PROJECT_DIR%" ^
    -DINCLUDE_GAME=OFF   

endlocal