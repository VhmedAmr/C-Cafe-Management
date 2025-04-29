@echo off
echo Setting up FLTK build environment...

:: Create FLTK directory if it doesn't exist
if not exist "C:\FLTK" mkdir "C:\FLTK"
cd "C:\FLTK"

:: Copy and extract FLTK source
echo Copying and extracting FLTK source...
copy "E:\adds\C++\College project test\downloads\fltk-1.3.8-source.tar.gz" .
powershell -Command "tar -xzf fltk-1.3.8-source.tar.gz"
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

:: Clean up
cd ..
cd ..
del fltk-1.3.8-source.tar.gz

echo FLTK build and installation complete!
echo Please set the environment variables manually:
echo 1. Add FLTK_ROOT = C:\FLTK\fltk-1.3.8
echo 2. Add C:\FLTK\fltk-1.3.8\bin to PATH
pause 