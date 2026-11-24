@echo off
rem Deletes every *.meta file in this bat's folder and all subfolders.
cd /d "%~dp0"
del /s /q /f "*.meta"
pause
