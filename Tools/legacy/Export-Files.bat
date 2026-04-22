@echo off

cd ..
cd Eclipse-Source

xcopy "Assets/DLLs\*" "Bin" /E /I /Y
xcopy "Assets/Libs\*" "Bin" /E /I /Y
xcopy "Assets/EngineAssets" "Bin/EngineAssets" /E /I /Y

robocopy "Source" "Bin/Headers" *.h *.inl *.hpp /S /E /R:0 /W:0
robocopy "Tools/Libs" "Bin/Libs" *.lib /S /E /R:0 /W:0