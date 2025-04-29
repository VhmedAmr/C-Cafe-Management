# Create FLTK directory if it doesn't exist
$fltkDir = "C:\FLTK"
if (-not (Test-Path $fltkDir)) {
    New-Item -ItemType Directory -Path $fltkDir
}

# Download FLTK 1.4.2
$url = "https://www.fltk.org/pub/fltk/snapshots/fltk-1.4.2-source.tar.gz"
$output = "$fltkDir\fltk-1.4.2-source.tar.gz"
Write-Host "Downloading FLTK 1.4.2..."
Invoke-WebRequest -Uri $url -OutFile $output

# Extract the archive
Write-Host "Extracting FLTK..."
tar -xzf $output -C $fltkDir

# Create build directory
$buildDir = "$fltkDir\fltk-1.4.2\build"
New-Item -ItemType Directory -Path $buildDir -Force

# Configure and build FLTK
Write-Host "Building FLTK..."
Set-Location $buildDir
& "C:\Program Files\CMake\bin\cmake.exe" -G "MinGW Makefiles" ..
& "C:\mingw64\bin\mingw32-make.exe"

# Clean up
Remove-Item $output

Write-Host "FLTK setup complete!"
Write-Host "Please set the environment variables manually:"
Write-Host "1. Add FLTK_ROOT = C:\FLTK\fltk-1.4.2"
Write-Host "2. Add C:\FLTK\fltk-1.4.2\build\bin to PATH"
Write-Host ""
Write-Host "To set environment variables:"
Write-Host "1. Open Windows Settings"
Write-Host "2. Search for 'Environment Variables'"
Write-Host "3. Click 'Edit the system environment variables'"
Write-Host "4. Click 'Environment Variables' button"
Write-Host "5. Under 'System Variables':"
Write-Host "   - Add new variable FLTK_ROOT with value C:\FLTK\fltk-1.4.2"
Write-Host "   - Edit Path variable and add C:\FLTK\fltk-1.4.2\build\bin" 