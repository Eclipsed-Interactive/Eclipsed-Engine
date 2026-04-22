@echo off
setlocal enabledelayedexpansion

:: Check for vswhere.exe
set "vswhere=C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"

if exist "!vswhere!" (
    :: Get the latest Visual Studio installation version
    for /f "delims=" %%i in ('"!vswhere!" -latest -property installationVersion') do (
        set "full_version=%%i"
    )
    
    :: Extract major version (first part before the first dot)
    for /f "tokens=1 delims=." %%a in ("!full_version!") do (
        set "major_version=%%a"
    )
    
    :: Determine release year based on major version
    if "!major_version!"=="18" (
        set "release_year=2026"
    ) else if "!major_version!"=="17" (
        set "release_year=2022"
    ) else if "!major_version!"=="16" (
        set "release_year=2019"
    ) else if "!major_version!"=="15" (
        set "release_year=2017"
    ) else if "!major_version!"=="14" (
        set "release_year=2015"
    ) else if "!major_version!"=="12" (
        set "release_year=2013"
    ) else if "!major_version!"=="11" (
        set "release_year=2012"
    ) else if "!major_version!"=="10" (
        set "release_year=2010"
    ) else (
        set "release_year=Unknown"
    )
    

    
) else (
    echo vswhere.exe not found. Trying registry method...
    
    :: Fallback to registry method for older Visual Studio versions
    for /f "tokens=1,2*" %%i in ('reg query "HKLM\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\SxS\VS7" 2^>nul') do (
        set "version_num=%%i"
        
        :: Extract major version
        for /f "tokens=1 delims=." %%a in ("!version_num!") do (
            set "major_version=%%a"
        )
        
        :: Determine release year based on major version
        if "!major_version!"=="18" (
            set "release_year=2026"
        ) else if "!major_version!"=="17" (
            set "release_year=2022"
        ) else if "!major_version!"=="16" (
            set "release_year=2019"
        ) else if "!major_version!"=="15" (
            set "release_year=2017"
        ) else if "!major_version!"=="14" (
            set "release_year=2015"
        ) else if "!major_version!"=="12" (
            set "release_year=2013"
        ) else if "!major_version!"=="11" (
            set "release_year=2012"
        ) else if "!major_version!"=="10" (
            set "release_year=2010"
        ) else (
            set "release_year=Unknown"
        )
        

    )
)



set VS_FULL_CMAKE=Visual Studio %major_version% %release_year%

:: Export variables to parent environment
endlocal & (
    set "VS_FULL_CMAKE=%VS_FULL_CMAKE%"
)