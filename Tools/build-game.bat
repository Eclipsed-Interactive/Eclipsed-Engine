@echo off

setlocal

call Game/generate-game-cmake.bat
call Game/build-game.bat

xcopy "../Eclipse-Source/Assets/DLLs" "../GameBuild" /E /Y /I

set /p PROJECT_PATH=< "../Eclipse-Source/Bin/.ini"
xcopy "%PROJECT_PATH%/Project/Assets" "../GameBuild/Project/Assets" /E /Y /I
xcopy "%PROJECT_PATH%/Project/Local" "../GameBuild/Project/Local" /E /Y /I
xcopy "%PROJECT_PATH%/Project/Settings" "../GameBuild/Project/Settings" /E /Y /I

endlocal

pause