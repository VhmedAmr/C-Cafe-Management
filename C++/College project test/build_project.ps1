# Set environment variables
$env:FLTK_ROOT = "C:\FLTK\fltk-1.4.2"
$env:Path += ";C:\mingw64\bin;C:\Program Files\CMake\bin"

# Set database connection parameters
$env:DB_HOST = "localhost"
$env:DB_PORT = "3306"
$env:DB_USER = "root"
$env:DB_PASS = "2525"
$env:DB_NAME = "cafe_management"

# Function to check if a command exists
function Test-Command($cmd) {
    return [bool](Get-Command -Name $cmd -ErrorAction SilentlyContinue)
}

# Check for required tools
if (-not (Test-Command "cmake")) {
    Write-Error "CMake not found. Please ensure CMake is installed and in your PATH."
    exit 1
}

if (-not (Test-Command "mingw32-make")) {
    Write-Error "MinGW not found. Please ensure MinGW is installed and in your PATH."
    exit 1
}

# Clean up old build directory
if (Test-Path "build") {
    Write-Host "Cleaning up old build directory..."
    Remove-Item -Recurse -Force "build"
}

# Create and enter build directory
Write-Host "Creating build directory..."
New-Item -ItemType Directory -Path "build" | Out-Null
Set-Location build

# Configure with CMake
Write-Host "Configuring project with CMake..."
cmake .. -G "MinGW Makefiles" `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_PREFIX_PATH="C:\FLTK\fltk-1.4.2" `
    -DMARIADB_SERVER_ROOT="C:\Program Files\MariaDB 11.7" `
    -DMARIADB_CONNECTOR_ROOT="C:\Program Files (x86)\MariaDB\MariaDB Connector C"

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit 1
}

# Build the project
Write-Host "Building project..."
cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit 1
}

# Create a release directory
if (-not (Test-Path "release")) {
    New-Item -ItemType Directory -Path "release" | Out-Null
}

# Copy the executable and required DLLs
Write-Host "Copying files to release directory..."
Copy-Item "CafeManagement.exe" "release\" -ErrorAction SilentlyContinue
Copy-Item "C:\FLTK\fltk-1.4.2\build\bin\*.dll" "release\" -ErrorAction SilentlyContinue
Copy-Item "C:\Program Files (x86)\MariaDB\MariaDB Connector C\lib\libmariadb.dll" "release\" -ErrorAction SilentlyContinue

Write-Host "Build completed successfully!"
Write-Host "The executable and required files are in the 'build/release' directory." 