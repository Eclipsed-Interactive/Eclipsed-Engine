@echo off

setlocal

echo === Begun: Generate engine cmake ===

call Tools/Setup/CheckVSVersion.bat
echo Generator: %VS_FULL_CMAKE%
echo.

cmake -G "%VS_FULL_CMAKE%"                 ^
    -T host=x64  ^
    -S .  ^
    -B Temp/Binary  ^
    -DECLIPSED_EDITOR=ON  ^
    -DPROJECT_DIR="%PROJECT_DIR%" ^
    -DINCLUDE_GAME=OFF   ^
    -DBUILD_GAME_PATH="${CMAKE_SOURCE_DIR}/Bin"

echo === Completed: Generate engine cmake ===

endlocal