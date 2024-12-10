conan install . --output-folder=build --build=missing
cmake -B build -G "Ninja" -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=RELEASE
$Env:CC='C:\ProgramData\mingw64\mingw64\bin\gcc.exe'
$Env:CXX='C:\ProgramData\mingw64\mingw64\bin\g++.exe'
# -I'C:\ProgramData\Include\
$Env:CXXFLAGS="-DCMAKE_COMPILATION_ -iquote ../gameEngine/ "
cmake --build build
cpack --config build/CPackConfig.cmake
