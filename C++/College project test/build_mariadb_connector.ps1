# Set environment variables
$env:Path += ";C:\mingw64\bin;C:\Program Files\CMake\bin"

# Change to MariaDB Connector/C directory
Set-Location "C:\mariadb-connector-c-3.4.5-src"

# Create and enter build directory
if (Test-Path "build") {
    Write-Host "Cleaning up old build directory..."
    Remove-Item -Recurse -Force "build"
}

Write-Host "Creating build directory..."
New-Item -ItemType Directory -Path "build" | Out-Null
Set-Location build

# Configure with CMake
Write-Host "Configuring MariaDB Connector/C..."
cmake .. -G "MinGW Makefiles" `
    -DCMAKE_BUILD_TYPE=Release `
    -DCMAKE_INSTALL_PREFIX="C:/mariadb-connector-c"

if ($LASTEXITCODE -ne 0) {
    Write-Error "CMake configuration failed."
    exit 1
}

# Build the project
Write-Host "Building MariaDB Connector/C..."
cmake --build . --config Release

if ($LASTEXITCODE -ne 0) {
    Write-Error "Build failed."
    exit 1
}

# Install
Write-Host "Installing MariaDB Connector/C..."
cmake --install .

Write-Host "MariaDB Connector/C build completed successfully!" 