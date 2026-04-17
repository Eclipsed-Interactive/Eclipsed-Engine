@echo off

echo === Begun: Setting up project ===
echo.

setlocal

cd Source/Externals/steamsdk/
call ./CurlSteam.bat

endlocal

echo.
echo === Completed: Setting up project ===

pause