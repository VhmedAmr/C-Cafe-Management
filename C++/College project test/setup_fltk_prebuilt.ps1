Write-Host "Setting up FLTK environment..."

# Create FLTK directory if it doesn't exist
if (-not (Test-Path "C:\FLTK")) {
    New-Item -ItemType Directory -Path "C:\FLTK"
}

# Download pre-built FLTK
Write-Host "Downloading pre-built FLTK..."
$url = "https://www.fltk.org/pub/fltk/snapshots/fltk-1.3.x-mingw64-dev.zip"
$output = "C:\FLTK\fltk.zip"
Invoke-WebRequest -Uri $url -OutFile $output

# Extract FLTK
Write-Host "Extracting FLTK..."
Expand-Archive -Path $output -DestinationPath "C:\FLTK" -Force

# Clean up
Remove-Item $output

Write-Host "FLTK setup complete!"
Write-Host "Please set the environment variables manually:"
Write-Host "1. Add FLTK_ROOT = C:\FLTK"
Write-Host "2. Add C:\FLTK\bin to PATH"

Write-Host "`nTo set environment variables:"
Write-Host "1. Open Windows Settings"
Write-Host "2. Search for 'Environment Variables'"
Write-Host "3. Click 'Edit the system environment variables'"
Write-Host "4. Click 'Environment Variables' button"
Write-Host "5. Under 'System Variables':"
Write-Host "   - Add new variable FLTK_ROOT with value C:\FLTK"
Write-Host "   - Edit Path variable and add C:\FLTK\bin"

Read-Host "Press Enter to continue" 