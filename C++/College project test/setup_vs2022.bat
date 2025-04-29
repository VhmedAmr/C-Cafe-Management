@echo off
echo Downloading Visual Studio 2022 installer...

:: Create downloads directory if it doesn't exist
if not exist "downloads" mkdir downloads
cd downloads

:: Download VS2022 Community installer
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://aka.ms/vs/17/release/vs_community.exe', 'vs_community.exe')"

:: Install VS2022 with necessary components
echo Installing Visual Studio 2022...
start /wait vs_community.exe --quiet --norestart --wait ^
    --add Microsoft.VisualStudio.Workload.NativeDesktop ^
    --add Microsoft.VisualStudio.Component.VC.Tools.x86.x64 ^
    --add Microsoft.VisualStudio.Component.Windows10SDK

:: Clean up
cd ..
rmdir /s /q downloads

echo Visual Studio 2022 installation complete!
pause 