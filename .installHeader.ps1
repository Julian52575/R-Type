# PowerShell Script: .installHeader.ps1

# Find all .hpp files excluding specified directories
$PWD = Get-Location
$PWD = "$PWD\"
Write-Host "PWD: $PWD"
$HPP_LIST = Get-ChildItem -Path "src/" -Recurse -Filter "*.hpp"

# Target directories
$INSTALL_PARENT_DIR = "C:\Program Files\Include"
$INSTALL_DIR = "$INSTALL_PARENT_DIR\rengine\"

# Clear target directory
if (Test-Path $INSTALL_DIR) {
    Remove-Item -Recurse -Force $INSTALL_DIR
}
New-Item -Path $INSTALL_DIR -Force -ItemType directory -Verbose
Copy-Item -Path "./*.hpp" -Destination $INSTALL_DIR -Verbose
try {
    # Copy headers
    #Write-Host "Hpp list: $HPP_LIST"
    foreach ($header in $HPP_LIST) {
        $aaa = $header.FullName.Substring($PWD.Length)
        Write-Host "Copying"
        Write-Host $header.fullName
        Write-Host "to"
        Write-Host "$INSTALL_DIR$aaa"
        try {
            $folder = $aaa
            Write-Host "Folder: $folder"
            New-Item -Path $INSTALL_DIR$folder -Force -ItemType directory -Verbose
        } catch {
            continue
        }
        Copy-Item -Path $header.FullName -Destination $INSTALL_DIR$aaa -Verbose
    }
} catch {
    Write-Warning "WARNING: Cannot copy dev headers into $INSTALL_DIR."
    exit 84
}

Write-Host "Installed dev headers in $INSTALL_DIR!"
