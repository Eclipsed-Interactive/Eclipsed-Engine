@echo off

setlocal

echo === Begun: Copying engine dlls ===

@REM cd ..
@REM cd Eclipse-Source

xcopy "Assets/DLLs\*" "Bin" /E /I /Y

echo === Completed: Copying engine dlls
endlocal