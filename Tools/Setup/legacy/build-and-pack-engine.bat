@echo off

set /p EXE_OUTPUT_DIR=Enter exe output directory: 

set "SRC=../Eclipse-Source"
set "BUILD=../Eclipse-Source/Library/Build-Binary"

cmake -G "Visual Studio 17 2022" -T host=x64 ^
 -S "%SRC%" ^
 -B "%BUILD%" ^
 -DECLIPSED_EDITOR=ON ^
 -DEXE_OUTPUT_DIR=%EXE_OUTPUT_DIR%

cmake --build "%BUILD%" --config Debug

xcopy "../Eclipse-Source/Tools/DLLs\*" "%EXE_OUTPUT_DIR%\" /E /I /Y


pause
