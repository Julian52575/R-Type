# PowerShell Script
$LIB = "librengine.dll"
$COPY_PATH = "C:\ProgramData\Include\rengine"
$RIGHT = $true

# Check if the library exists and is executable
if (-not (Test-Path $LIB -PathType Leaf)) {
    Write-Host "$LIB not found, starting compile script."
    # Run compile script
    ./compile.ps1
}

Write-Host "Installing dev header..."
# Run install header script
./.installHeader.ps1

Write-Host "Installing lib..."
if (-not (Test-Path $COPY_PATH)) {
    New-Item -ItemType Directory -Path $COPY_PATH
}
try {
    Copy-Item $LIB -Destination $COPY_PATH -Force
    Write-Host "$LIB has been installed!"
} catch {
    $RIGHT = $false
}

if (-not $RIGHT) {
    Write-Warning "WARNING: Cannot install $LIB to your path."
    Write-Warning "Move $LIB yourself."
}

$currentPath = [System.Environment]::GetEnvironmentVariable("PATH", [System.EnvironmentVariableTarget]::User)
# Check if the path is already in the PATH variable
if ($currentPath -notlike "*$pathToAdd*") {
    # Add the new path to the current PATH variable
    $COPY_PATH = "$currentPath;$pathToAdd"
    
    # Set the new PATH variable
    [System.Environment]::SetEnvironmentVariable("PATH", $COPY_PATH, [System.EnvironmentVariableTarget]::User)
    
    Write-Host "Path added to environment variable."
} else {
    Write-Host "Path already exists in the environment variable."
}