#!/bin/bash
LIB_NAME=librengine.so

conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=RELEASE
if cmake --build build ; then
    chmod a+x build/$LIB_NAME
    mv build/$LIB_NAME .
    echo "Succesfully built $LIB_NAME !"
fi
#cpack --config build/CPackConfig.cmake
