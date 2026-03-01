@echo off
setlocal

:: Target folder
set LOCAL_DIR=..\Eclipse-Source\Bin

:: Make folder if it doesn't exist
if not exist "%LOCAL_DIR%" (
    echo Folder not found, creating: %LOCAL_DIR%
    mkdir "%LOCAL_DIR%"
)

pushd "%LOCAL_DIR%"

:: Target file
set PROJECT_FILE=.ini

:: Create the file if it doesn't exist
if not exist "%PROJECT_FILE%" (
    echo File not found, creating empty file: %PROJECT_FILE%
    echo.>"%PROJECT_FILE%"
)

:: Read the first line of the file into PROJECT_DIR
set PROJECT_DIR=
for /f "usebackq delims=" %%A in ("%PROJECT_FILE%") do (
    set PROJECT_DIR=%%A
    goto :afterRead
)

:afterRead
if "%PROJECT_DIR%"=="" (
	echo No path found inside %LOCAL_DIR%\.ini
	goto :SetProjectDir
)
if not exist "%PROJECT_DIR%" (
	echo Path: "%PROJECT_DIR%", does not contain an .ini file
	goto :SetProjectDir
)

echo %PROJECT_DIR% > "%PROJECT_FILE%"

popd

goto afterSet

:SetProjectDir
set /p PROJECT_DIR=Enter the path to the project:
goto :afterRead

:afterSet










cmake -G "Visual Studio 17 2022"                 ^
    -T host=x64  ^
    -S ../Eclipse-Source/  ^
    -B ../Eclipse-Source/Library/Binary  ^
    -DECLIPSED_EDITOR=OFF  ^
    -DPROJECT_DIR="%PROJECT_DIR%" ^
    -DINCLUDE_GAME=OFF                           
    
cd ..
cd Eclipse-Source

xcopy "Assets/DLLs\*" "Bin" /E /I /Y
@REM xcopy "Assets/Libs\*" "Bin" /E /I /Y
@REM xcopy "Assets/EngineAssets" "Bin/EngineAssets" /E /I /Y

set SOURCE=Source
set DEST=Bin/Headers

@REM robocopy "Source" "Bin/Headers" *.h *.inl *.hpp /S /E /R:0 /W:0
@REM robocopy "Tools/Libs" "Bin/Libs" *.lib /S /E /R:0 /W:0


pause
@REM rem Hide the build folder.
@REM attrib +h "../Eclipse-Source/build"