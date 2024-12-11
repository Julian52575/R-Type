# PowerShell Script: compile.ps1
$GENERATED_LIB_NAME = "rengine.dll"
$LIB_NAME = "librengine.dll"
$PROJECT_FILE = "build\build.ninja"

# Clean the build directory
if (Test-Path "build") {
    Remove-Item -Recurse -Force "build"
}

# Run Conan to install dependencies
conan install conanfile.txt --output-folder=build --build=missing 

# Run CMake to generate the solution file
cmake -B build -G "Visual Studio 17 2022" `
      -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" `
      -DCMAKE_BUILD_TYPE=Release `

cmake --build build --config Release

$FULL_PATH = "build\Release\$GENERATED_LIB_NAME"
Write-Host "Checking if $FULL_PATH exists..."

# Check if the .dll was successfully built
if (Test-Path $FULL_PATH) {
    Write-Host "Moving lib"
    Move-Item -Path $FULL_PATH -Destination $LIB_NAME -Force
    if (Test-Path "$LIB_NAME") {
        Write-Host "Successfully moved $LIB_NAME!"
    } else {
        Write-Error "Failed to move $LIB_NAME. Aborting."
    }
    Write-Host "Successfully built $LIB_NAME!"
} else {
    Write-Error "Failed to build $LIB_NAME. Check the build logs for errors."
}