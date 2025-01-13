#!/bin/bash
LIB="librengine.so"
LIB_GRAPHIC=librengine_graphics_sfml.so
LIB_SERVER=librengine_server.so

RIGHT=True

if ! test -x $LIB; then
    echo "$LIB not found, starting compile script."
    ./compile.sh
fi

echo "Installing lib..."
if ! sudo mv $LIB /usr/lib --verbose; then
    RIGHT=False

else
    echo "$LIB has been installed !"
fi

if ! sudo mv $LIB_GRAPHIC /usr/lib --verbose ; then
    RIGHT=False

else
    echo "$LIB_GRAPHIC has been installed !"
fi

if ! sudo mv $LIB_SERVER /usr/lib --verbose; then
    RIGHT=False

else
    echo "$LIB_SERVER has been installed !"
fi


echo "Installing dev header..."
sudo ./.installHeader.sh
if [ $RIGHT == False ]; then
    echo "WARNING: Cannot install /usr/lib/$LIB."
    echo "Move $LIB into /usr/lib yourself."
fi
