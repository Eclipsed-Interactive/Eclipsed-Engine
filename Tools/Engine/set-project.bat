@echo off
setlocal

:: Target folder
@REM set LOCAL_DIR=../Eclipse-Source/Bin

@REM :: Make folder if it doesn't exist
@REM if not exist "%LOCAL_DIR%" (
@REM     echo Folder not found, creating: %LOCAL_DIR%
@REM     mkdir "%LOCAL_DIR%"
@REM )

@REM pushd "%LOCAL_DIR%"

@REM :: Target file
@REM set PROJECT_FILE=.ini

@REM :: Create the file if it doesn't exist
@REM if not exist "%PROJECT_FILE%" (
@REM     echo File not found, creating empty file: %PROJECT_FILE%
@REM     echo.>"%PROJECT_FILE%"
@REM )

@REM :: Read the first line of the file into PROJECT_DIR
@REM set PROJECT_DIR=
@REM for /f "usebackq delims=" %%A in ("%PROJECT_FILE%") do (
@REM     set PROJECT_DIR=%%A
@REM     goto :afterRead
@REM )

@REM :afterRead
@REM if "%PROJECT_DIR%"=="" (
@REM 	echo No path found inside %LOCAL_DIR%\.ini
@REM 	goto :SetProjectDir
@REM )
@REM if not exist "%PROJECT_DIR%" (
@REM 	echo Path: "%PROJECT_DIR%", does not contain an .ini file
@REM 	goto :SetProjectDir
@REM )

@REM echo %PROJECT_DIR%> "%PROJECT_FILE%"

@REM popd

@REM goto afterSet

@REM :SetProjectDir
@REM for /f "delims=" %%i in ('powershell -Command "Add-Type -AssemblyName System.Windows.Forms; $folder = New-Object System.Windows.Forms.FolderBrowserDialog; $folder.Description = 'Select a folder'; $folder.ShowNewFolderButton = $true; if($folder.ShowDialog() -eq 'OK') { $folder.SelectedPath }"') do set "PROJECT_DIR=%%i"

@REM goto :afterRead

@REM :afterSet
endlocal
