#!/bin/bash
LIB="/usr/lib/librengine.so"
HEADER_DIR="/usr/include/rengine"

if $(test -x $LIB); then
    rm --verbose $LIB || true
fi

if $(test -d $HEADER_DIR); then
    rm -rf $HEADER_DIR --verbose | grep -v '/usr/include/rengine/src'  || true
fi
