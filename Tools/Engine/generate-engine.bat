@echo off
setlocal

call generate-engine-cmake.bat
call copy-engine-dlls.bat

endlocal