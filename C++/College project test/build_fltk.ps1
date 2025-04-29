Write-Host "Setting up FLTK build environment..."

# Set CMake path
$CMAKE_PATH = "C:\Program Files\CMake\bin\cmake.exe"

# Create FLTK directory if it doesn't exist
if (-not (Test-Path "C:\FLTK")) {
    New-Item -ItemType Directory -Path "C:\FLTK"
}
Set-Location "C:\FLTK"

# Copy and extract FLTK source
Write-Host "Copying and extracting FLTK source..."
Copy-Item "E:\adds\C++\College project test\downloads\fltk-1.3.8-source.tar.gz" -Destination "."
tar -xzf fltk-1.3.8-source.tar.gz
Set-Location fltk-1.3.8

# Update CMakeLists.txt to use newer CMake version
Write-Host "Updating CMake version requirement..."
$cmakeListsContent = Get-Content -Path "CMakeLists.txt"
$cmakeListsContent = $cmakeListsContent -replace "cmake_minimum_required\(VERSION [0-9.]+\)", "cmake_minimum_required(VERSION 3.5)"
Set-Content -Path "CMakeLists.txt" -Value $cmakeListsContent

# Create build directory
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}
Set-Location build

# Configure with CMake
Write-Host "Configuring FLTK with CMake..."
& $CMAKE_PATH -G "MinGW Makefiles" -DCMAKE_INSTALL_PREFIX=C:/FLTK/fltk-1.3.8 -DCMAKE_BUILD_TYPE=Release -DOPTION_USE_SYSTEM_LIBJPEG=OFF -DOPTION_USE_SYSTEM_LIBPNG=OFF -DOPTION_USE_SYSTEM_ZLIB=OFF ..

# Build and install
Write-Host "Building FLTK..."
& $CMAKE_PATH --build . --config Release
& $CMAKE_PATH --install .

# Clean up
Set-Location ..
Set-Location ..
Remove-Item fltk-1.3.8-source.tar.gz

Write-Host "FLTK build and installation complete!"
Write-Host "Please set the environment variables manually:"
Write-Host "1. Add FLTK_ROOT = C:\FLTK\fltk-1.3.8"
Write-Host "2. Add C:\FLTK\fltk-1.3.8\bin to PATH"
Read-Host "Press Enter to continue" 