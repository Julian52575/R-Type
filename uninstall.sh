#!/bin/bash
LIB="/usr/lib/librengine.so"
LIB_GRAPHIC="/usr/lib/librengine_graphics_sfml.so"
LIB_SERVER="/usr/lib/librengine_server.so"
HEADER_DIR="/usr/include/rengine"

if $(test -x $LIB); then
    sudo rm --verbose $LIB || true
    sudo rm --verbose $LIB_GRAPHIC || true
    sudo rm --verbose $LIB_SERVER || true
fi

if $(test -d $HEADER_DIR); then
    sudo rm -rf $HEADER_DIR --verbose | grep -v '/usr/include/rengine/src'  || true
fi
