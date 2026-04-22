@echo off
setlocal

call set-project.bat
call generate-engine-cmake.bat
call copy-engine-dlls.bat

endlocal