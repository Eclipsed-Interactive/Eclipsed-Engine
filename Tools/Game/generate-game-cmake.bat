@echo off

setlocal

cmake -G "Visual Studio 17 2022"                 ^
    -T host=x64  ^
    -S ../Eclipse-Source/  ^
    -B ../Eclipse-Source/Library/Game-Binary  ^
    -DECLIPSED_EDITOR=OFF  ^
    -DINCLUDE_GAME=ON   ^
    -DBUILD_GAME_PATH="%CD%/../GameBuild"

endlocal