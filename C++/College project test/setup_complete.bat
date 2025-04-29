@echo off
echo Running setup with administrator privileges...

:: Check for admin privileges
net session >nul 2>&1
if %errorLevel% == 0 (
    echo Already running with administrator privileges
) else (
    echo Requesting administrator privileges...
    powershell -Command "Start-Process '%~dpnx0' -Verb RunAs"
    exit /b
)

:: Create downloads directory
if not exist "downloads" mkdir downloads
cd downloads

echo Downloading MinGW-w64...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/niXman/mingw-builds-binaries/releases/download/13.2.0-rt_v11-rev1/x86_64-13.2.0-release-posix-seh-ucrt-rt_v11-rev1.7z', 'mingw.7z')"

echo Downloading 7-Zip...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://www.7-zip.org/a/7zr.exe', '7zr.exe')"

echo Installing 7-Zip...
start /wait 7zr.exe /S

echo Extracting MinGW-w64...
if not exist "C:\mingw64" mkdir "C:\mingw64"
"C:\Program Files\7-Zip\7z.exe" x mingw.7z -oC:\mingw64

echo Downloading CMake...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/Kitware/CMake/releases/download/v3.27.7/cmake-3.27.7-windows-x86_64.msi', 'cmake.msi')"

echo Downloading FLTK...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://www.fltk.org/pub/fltk/1.3.8/fltk-1.3.8-source.tar.gz', 'fltk.tar.gz')"

echo Installing CMake...
start /wait msiexec /i cmake.msi /qn ADD_CMAKE_TO_PATH=System

echo Setting up FLTK...
if not exist "%USERPROFILE%\FLTK" mkdir "%USERPROFILE%\FLTK"
tar -xf fltk.tar.gz -C "%USERPROFILE%\FLTK"

echo Setting environment variables...
setx FLTK_ROOT "%USERPROFILE%\FLTK\fltk-1.3.8" /M
setx PATH "%PATH%;C:\mingw64\bin;%USERPROFILE%\FLTK\fltk-1.3.8\lib" /M

echo Cleaning up...
cd ..
rmdir /s /q downloads

echo Setup complete!
echo Please restart your computer for all changes to take effect.
pause 