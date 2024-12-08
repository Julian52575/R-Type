# PowerShell Script
$LIB = "librengine.sln"
$RIGHT = $true

# Check if the library exists and is executable
if (-not (Test-Path $LIB -PathType Leaf)) {
    Write-Host "$LIB not found, starting compile script."
    # Run compile script
    ./compile.ps1
}

Write-Host "Installing lib..."
try {
    Copy-Item $LIB -Destination "C:\Windows\System32" -Verbose -ErrorAction Stop
    Write-Host "$LIB has been installed!"
} catch {
    $RIGHT = $false
}

Write-Host "Installing dev header..."
# Run install header script
./.installHeader.ps1

if (-not $RIGHT) {
    Write-Warning "WARNING: Cannot install $LIB to /usr/lib."
    Write-Warning "Move $LIB to /usr/lib manually."
}

