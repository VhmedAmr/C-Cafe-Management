@echo off
echo Setting up development environment...

:: Create downloads directory
if not exist "downloads" mkdir downloads
cd downloads

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
setx FLTK_ROOT "%USERPROFILE%\FLTK\fltk-1.3.8"
setx PATH "%PATH%;%USERPROFILE%\FLTK\fltk-1.3.8\lib"

echo Cleaning up...
cd ..
rmdir /s /q downloads

echo Setup complete!
echo Please make sure you have Visual Studio 2022 installed with C++ development tools.
echo You can download it from: https://visualstudio.microsoft.com/vs/community/
pause 