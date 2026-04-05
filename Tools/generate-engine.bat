@echo off
setlocal

echo === Running set-project.bat ===
call Engine/set-project.bat

echo.
echo === Verifying .ini file ===
if exist "../Eclipse-Source/Bin/.ini" (
    echo SUCCESS: .ini file found
    type "../Eclipse-Source/Bin/.ini"
) else (
    echo ERROR: .ini file not found in ../Eclipse-Source/Bin/
    dir "../Eclipse-Source/Bin" 2>nul || echo Bin directory not found
)

echo.

echo === Running generate-engine-cmake.bat ===
call Engine/generate-engine-cmake.bat

call Engine/copy-engine-dlls.bat

endlocal
pause