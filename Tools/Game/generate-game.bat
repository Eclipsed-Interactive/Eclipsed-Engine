@echo off
setlocal

call set-project.bat
call build-engine-cmake.bat
call copy-engine-dlls.bat

endlocal