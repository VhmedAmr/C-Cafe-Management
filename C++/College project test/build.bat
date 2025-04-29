@echo off
echo Building project...

:: Create build directory if it doesn't exist
if not exist "build" mkdir build
cd build

:: Configure with MinGW
cmake -G "MinGW Makefiles" ..

:: Build the project
cmake --build .

echo Build complete!
cd ..
pause 