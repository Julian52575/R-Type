# PowerShell Script: .installHeader.ps1

# Find all .hpp files excluding specified directories
$HPP_LIST = Get-ChildItem -Recurse -Include *.hpp | 
    Where-Object { -not $_.FullName -match "\\lol\\|\\build\\|\\rengine\\" }

# Target directories
$INSTALL_PARENT_DIR = "C:\Program Files\Include"
$INSTALL_DIR = "$INSTALL_PARENT_DIR\rengine"

# Rights check
$TEST_FILE = "rengine.test"
New-Item -ItemType File -Path $TEST_FILE | Out-Null

try {
    Move-Item -Path $TEST_FILE -Destination $INSTALL_PARENT_DIR
    Remove-Item -Path "$INSTALL_PARENT_DIR\$TEST_FILE" -ErrorAction SilentlyContinue
} catch {
    Write-Warning "WARNING: Cannot copy dev headers into $INSTALL_DIR."
    $INSTALL_DIR = (Get-Location).Path + "\rengine"
    Write-Warning "Installing in $INSTALL_DIR instead."
    Write-Warning "Copy $INSTALL_DIR to $INSTALL_PARENT_DIR yourself."
}

# Clear target directory
if (Test-Path $INSTALL_DIR) {
    Remove-Item -Recurse -Force $INSTALL_DIR
}
New-Item -ItemType Directory -Path $INSTALL_DIR | Out-Null

# Copy headers
foreach ($header in $HPP_LIST) {
    $targetDir = Join-Path $INSTALL_DIR ($header.DirectoryName -replace [regex]::Escape((Get-Location).Path), "")
    New-Item -ItemType Directory -Force -Path $targetDir | Out-Null
    Copy-Item -Path $header.FullName -Destination $targetDir
}

Write-Host "Installed dev headers in $INSTALL_DIR!"

if ($INSTALL_DIR -ne "$INSTALL_PARENT_DIR\rengine") {
    Write-Warning "WARNING: Move $INSTALL_DIR to $INSTALL_PARENT_DIR\rengine"
}
