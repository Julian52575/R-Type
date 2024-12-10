#!/bin/bash
LIB_NAME=librengine.so

if ! dpkg -s python3-venv > /dev/null 2>&1; then
    echo "python3-venv is not installed. Installing..."
    sudo apt-get install python3-venv
fi

python3 -m venv venvconan
source venvconan/bin/activate
pip3 install conan

if ! conan --version > /dev/null; then
    echo "Installing conan with pip..."
    if ! sudo pip install conan --no-warn-conflicts; then
        exit 84
    fi
fi

conan install . --output-folder=build --build=missing -c tools.system.package_manager:mode=install
cmake -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=RELEASE
if cmake --build build ; then
    chmod a+x build/$LIB_NAME
    mv build/$LIB_NAME .
    echo "Succesfully built $LIB_NAME !"
fi
#cpack --config build/CPackConfig.cmake

deactivate
rm -rf venvconan