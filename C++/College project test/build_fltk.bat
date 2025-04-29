@echo off
echo Setting up FLTK build environment...

:: Create FLTK directory if it doesn't exist
if not exist "C:\FLTK" mkdir "C:\FLTK"
cd "C:\FLTK"

:: Download FLTK source
echo Downloading FLTK source...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://www.fltk.org/pub/fltk/1.3.8/fltk-1.3.8-source.tar.gz', 'fltk-source.tar.gz')"

:: Extract FLTK source
echo Extracting FLTK source...
powershell -Command "tar -xzf fltk-source.tar.gz"
cd fltk-1.3.8

:: Create build directory
mkdir build
cd build

:: Configure with CMake
echo Configuring FLTK with CMake...
cmake -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/FLTK/fltk-1.3.8 -DCMAKE_BUILD_TYPE=Release -DOPTION_USE_SYSTEM_LIBJPEG=OFF -DOPTION_USE_SYSTEM_LIBPNG=OFF -DOPTION_USE_SYSTEM_ZLIB=OFF ..

:: Build and install
echo Building FLTK...
cmake --build . --config Release
cmake --install .

:: Set environment variables
echo Setting up environment variables...
setx FLTK_ROOT "C:\FLTK\fltk-1.3.8"
setx PATH "%PATH%;C:\FLTK\fltk-1.3.8\bin"

:: Clean up
cd ..
cd ..
del fltk-source.tar.gz

echo FLTK build and installation complete!
echo Please restart your terminal for the changes to take effect.
pause 