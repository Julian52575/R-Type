#!/bin/bash
LIB="librengine.so"

if ! test -x $LIB; then
    echo "$LIB not found, starting compile script."
    ./compile.sh
fi

echo "I'm asking for sudo rights to execute the following command:"
echo "cp $LIB /usr/lib"
sudo cp $LIB /usr/lib --verbose
echo "Done !"
