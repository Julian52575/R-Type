conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=RELEASE
cmake --build build
cpack --config build/CPackConfig.cmake
