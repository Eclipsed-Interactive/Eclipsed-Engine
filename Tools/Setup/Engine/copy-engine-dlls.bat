@echo off

setlocal

echo === Begun: Copying engine dlls ===

xcopy "Thirdparty/DLLs\*" "Bin" /E /I /Y

echo === Completed: Copying engine dlls
endlocal