@echo off
echo Setting up FLTK...

:: Create FLTK directory if it doesn't exist
if not exist "C:\FLTK" mkdir "C:\FLTK"

:: Download FLTK
echo Downloading FLTK...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://github.com/fltk/fltk/releases/download/release-1.3.8/fltk-1.3.8-win64.zip', 'C:\FLTK\fltk.zip')"

:: Extract FLTK
echo Extracting FLTK...
powershell -Command "Expand-Archive -Path 'C:\FLTK\fltk.zip' -DestinationPath 'C:\FLTK' -Force"

:: Set environment variables
echo Setting up environment variables...
setx FLTK_ROOT "C:\FLTK\fltk-1.3.8"
setx PATH "%PATH%;C:\FLTK\fltk-1.3.8\bin"

:: Clean up
del "C:\FLTK\fltk.zip"

echo FLTK setup complete!
echo Please restart your terminal for the changes to take effect.
pause 