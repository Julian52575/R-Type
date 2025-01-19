#!/bin/bash
RED='\033[0;31m'
NC='\033[0m' # No Color
BIN_NAME="rtype"
FLAGS=""
BUILD_TYPE="RELEASE"

if ! dpkg -s python3-venv > /dev/null 2>&1; then
    echo "python3-venv is not installed. Installing..."
    sudo apt-get install python3-venv
fi

python3 -m venv venvconan
source venvconan/bin/activate
pip3 install conan

if ! conan install . --output-folder=build --build=missing -s build_type=Release; then
    echo "Conan failure. Aborting..."
    exit 84
fi
if ! conan install . --output-folder=build --build=missing -s build_type=Debug; then
    echo "Conan failure. Aborting..."
    exit 84
fi

if [[ "$1" == "debug" ]]; then
    echo -e "$RED Compiling debug binary.$NC"
    BUILD_TYPE="DEBUG"  # debug build
fi
cmake -B build -DCMAKE_TOOLCHAIN_FILE="build/conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=$BUILD_TYPE
if cmake --build build;then
    chmod a+x build/$BIN_NAME
    mv build/$BIN_NAME $BIN_NAME
    echo "$BIN_NAME has been succesfuly built !"
else
    echo "Build failed. Do you have librengine.so in your path ? /usr/lib"
    exit 84
fi

deactivate
rm -rf venvconan
