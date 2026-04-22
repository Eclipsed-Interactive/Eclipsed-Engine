@echo off

echo === Begun: Setting up project ===
echo.

setlocal

cd Source/Externals/steamsdk/
call ./CurlSteam.bat

echo.
echo === Begun: Installing python and dependancies ===
winget install Python.Python.3.14
py -m pip install pillow
py -m pip install numpy
echo === Completed: Installing python and dependancies ===

endlocal

echo.
echo === Completed: Setting up project ===

pause