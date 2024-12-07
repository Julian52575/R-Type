#!/bin/bash
LIB_NAME=librengine.so

if ! conan --version > /dev/null; then
    echo "Installing conan with pip..."
    if ! sudo pip install conan --no-warn-conflicts; then
        exit 84
    fi
fi

conan install . --output-folder=build --build=missing
cmake -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=RELEASE
if cmake --build build ; then
    chmod a+x build/$LIB_NAME
    mv build/$LIB_NAME .
    echo "Succesfully built $LIB_NAME !"
fi
#cpack --config build/CPackConfig.cmake
