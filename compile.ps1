# PowerShell Script: compile.ps1
$LIB_NAME = "librengine.dll"
$PROJECT_FILE = "build\build.ninja"

# Run Conan to install dependencies
conan install . --output-folder=build --build=missing

# Run CMake to generate the solution file
cmake -B build -G "Ninja" `
      -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" `
      -DCMAKE_BUILD_TYPE=Release `
      -DCMAKE_C_COMPILER=gcc `
      -DCMAKE_CXX_COMPILER=g++

# Check if the .sln file exists
if (-Not (Test-Path $PROJECT_FILE)) {
    Write-Error "Project file ($PROJECT_FILE) not generated. Aborting."
    exit 84
}

# Build the solution using Ninja
Invoke-Expression 'ninja -C build'

# Check if the .dll was successfully built
if (Test-Path "build\Release\$LIB_NAME") {
    Move-Item -Path "build\Release\$LIB_NAME" -Destination "."
    Write-Host "Successfully built $LIB_NAME!"
} else {
    Write-Error "Failed to build $LIB_NAME. Check the build logs for errors."
}
