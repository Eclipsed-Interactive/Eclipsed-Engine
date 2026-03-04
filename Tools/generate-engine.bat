@echo off
setlocal

call Engine/set-project.bat
call Engine/generate-engine-cmake.bat
call Engine/copy-engine-dlls.bat

endlocal

pause