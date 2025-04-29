@echo off
echo Setting up MinGW-w64 development environment...

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

echo Setting environment variables...
setx PATH "%PATH%;C:\mingw64\bin"

echo Cleaning up...
cd ..
rmdir /s /q downloads

echo MinGW-w64 setup complete!
echo Please restart your terminal for the changes to take effect.
pause 