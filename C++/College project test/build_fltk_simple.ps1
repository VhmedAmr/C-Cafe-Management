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

# Create build directory
if (-not (Test-Path "build")) {
    New-Item -ItemType Directory -Path "build"
}
Set-Location build

# Configure with CMake (simpler configuration)
Write-Host "Configuring FLTK with CMake..."
& $CMAKE_PATH .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release

# Build
Write-Host "Building FLTK..."
mingw32-make

# Set environment variables
Write-Host "`nFLTK build complete!"
Write-Host "Please set the environment variables manually:"
Write-Host "1. Open Windows Settings"
Write-Host "2. Search for 'Environment Variables'"
Write-Host "3. Click 'Edit the system environment variables'"
Write-Host "4. Click 'Environment Variables' button"
Write-Host "5. Under 'System Variables':"
Write-Host "   - Add new variable FLTK_ROOT with value C:\FLTK\fltk-1.3.8"
Write-Host "   - Edit Path variable and add C:\FLTK\fltk-1.3.8\bin"

Read-Host "Press Enter to continue" 