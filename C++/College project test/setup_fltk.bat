@echo off
echo Downloading FLTK...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://www.fltk.org/pub/fltk/1.3.8/fltk-1.3.8-msvc2019-win64.zip', 'fltk.zip')"
echo Extracting FLTK...
if not exist "%USERPROFILE%\FLTK" mkdir "%USERPROFILE%\FLTK"
powershell -Command "Expand-Archive -Path 'fltk.zip' -DestinationPath '%USERPROFILE%\FLTK' -Force"
echo Setting up environment variables...
setx FLTK_ROOT "%USERPROFILE%\FLTK\fltk-1.3.8-msvc2019-win64"
setx PATH "%PATH%;%USERPROFILE%\FLTK\fltk-1.3.8-msvc2019-win64\bin"
echo FLTK setup complete!
del fltk.zip
pause 