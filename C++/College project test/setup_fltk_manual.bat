@echo off
echo Setting up FLTK...

:: Create FLTK directory if it doesn't exist
if not exist "%USERPROFILE%\FLTK" mkdir "%USERPROFILE%\FLTK"

:: Extract FLTK
echo Extracting FLTK...
powershell -Command "Expand-Archive -Path 'fltk\fltk-source.tar.gz' -DestinationPath '%USERPROFILE%\FLTK' -Force"

:: Set environment variables
echo Setting up environment variables...
setx FLTK_ROOT "%USERPROFILE%\FLTK\fltk-1.3.8"
setx PATH "%PATH%;%USERPROFILE%\FLTK\fltk-1.3.8\bin"

echo FLTK setup complete!
echo Please restart your terminal for the changes to take effect.
pause 