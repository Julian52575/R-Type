#!/bin/bash
LIB="librengine.so"
LIB_GRAPHIC=librengine_graphics_sfml.so
LIB_SERVER=librengine_server.so

RIGHT=True

if ! test -x $LIB; then
    echo "$LIB not found, starting compile script."
    ./compile.sh
fi

echo "Installing libraries..."
if ! sudo mv $LIB /usr/lib --verbose; then
    RIGHT=False
fi

if ! sudo mv $LIB_GRAPHIC /usr/lib --verbose ; then
    RIGHT=False
fi

if ! sudo mv $LIB_SERVER /usr/lib --verbose; then
    RIGHT=False
fi

echo "Installing dev headers..."
sudo ./.installHeader.sh
if [ $RIGHT == False ]; then
    echo "ERROR: Cannot install /usr/lib/$LIB."
    exit 84
fi
