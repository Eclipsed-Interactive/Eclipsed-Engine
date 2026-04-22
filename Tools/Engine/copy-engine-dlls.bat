@echo off

setlocal
cd ..
cd ..
cd Eclipse-Source

xcopy "Assets/DLLs\*" "Bin" /E /I /Y
endlocal