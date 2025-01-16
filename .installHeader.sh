#!/bin/bash
# 
HPP_LIST=$(find . -name "*.hpp" -not -path "./lol/*.hpp" -not -path "./build/*.hpp" -not -path "./rengine/*.hpp" | tr " " "\n")
HPP_LIST=(${HPP_LIST// /\n})
TPP_LIST=$(find . -name "*.tpp" -not -path "./lol/*.tpp" -not -path "./build/*.tpp" -not -path "./rengine/*.tpp" | tr " " "\n")
# if array len is superior to one, sanitarize it
if (( ${#TPP_LIST[@]} > 1 )); then
    TPP_LIST=($TPP_LIST// /\n})
fi

INSTALL_PARENT_DIR=/usr/include
INSTALL_DIR="$INSTALL_PARENT_DIR/rengine"

# Rights check
TEST_FILE="rengine.test"
touch $TEST_FILE
if ! mv $TEST_FILE $INSTALL_PARENT_DIR; then
    echo "ERROR: No access dev headers into $INSTALL_DIR."
    exit 84
fi
rm $TEST_FILE 2> /dev/null || true
rm "$INSTALL_PARENT_DIR/$TEST_FILE" 2> /dev/null || true

# Clear target directory
rm -rf $INSTALL_DIR 2> /dev/null || true

# Copy each header in INSTALL_DIR
for header in "${HPP_LIST[@]}"
do
    mkdir -p "$INSTALL_DIR/$(dirname $header)"
    cp -a $header "$INSTALL_DIR/$header"
done
for header in "${TPP_LIST[@]}"
do
    mkdir -p "$INSTALL_DIR/$(dirname $header)"
    cp -a $header "$INSTALL_DIR/$header"
done

echo "Installed dev headers in $INSTALL_DIR !"
