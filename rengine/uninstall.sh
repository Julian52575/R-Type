#!/bin/bash
LIB="/usr/lib/librengine.so"
HEADER_DIR="/usr/include/rengine"

if $(test -x $LIB); then
    sudo rm --verbose $LIB || true
fi

if $(test -d $HEADER_DIR); then
    sudo rm -rf $HEADER_DIR --verbose | grep -v '/usr/include/rengine/src'  || true
fi
