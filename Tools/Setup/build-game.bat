@echo off

setlocal

call Game/generate-game-cmake.bat
call Game/build-game.bat Release

set /p PROJECT_PATH=< "../Eclipse-Source/Bin/.ini"
xcopy "../Eclipse-Source/Assets/DLLs" "%PROJECT_PATH%/Bin" /E /Y /I

xcopy "%PROJECT_PATH%/Project/Assets" "%PROJECT_PATH%/Bin/Project/Assets" /E /Y /I
xcopy "%PROJECT_PATH%/Project/Local" "%PROJECT_PATH%/Bin/Project/Local" /E /Y /I
xcopy "%PROJECT_PATH%/Project/Settings" "%PROJECT_PATH%/Bin/Project/Settings" /E /Y /I

endlocal

pause