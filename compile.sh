#!/bin/bash
BIN_NAME="rtype"

conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=RELEASE
if cmake --build build;then
    chmod a+x build/$BIN_NAME
    mv build/$BIN_NAME $BIN_NAME
    echo "$BIN_NAME has been succesfuly built !"
else
    echo "Build failed. Do you have librengine.so in your path ? /usr/lib"
    exit 84
fi
