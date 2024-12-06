#!/bin/bash
# 
HPP_LIST=$(find . -name "*.hpp" -not -path "./lol/*.hpp" -not -path "./build/*.hpp" | tr " " "\n")
HPP_LIST=(${HPP_LIST// /\n})
INSTALL_PARENT_DIR=/usr/include
INSTALL_DIR="$INSTALL_PARENT_DIR/rengine"

# Rights check
TEST_FILE="rengine.test"
touch $TEST_FILE
if ! mv $TEST_FILE $INSTALL_PARENT_DIR; then
    echo "Cannot copy file into $INSTALL_DIR."
    INSTALL_DIR="$(pwd)/rengine"
    echo "Installing in $INSTALL_DIR instead. Copy $INSTALL_DIR into $INSTALL_DIR yourself."
    exit 84
fi
rm $TEST_FILE 2> /dev/null || true
rm "$INSTALL_PARENT_DIR/$TEST_FILE" 2> /dev/null || true

# Clear target directory
rm -rf $INSTALL_DIR --verbose
for header in "${HPP_LIST[@]}"
do
    sudo mkdir -p "$INSTALL_DIR/$(dirname $header)" --verbose
    sudo cp $header "$INSTALL_DIR/$header" --verbose
done
echo "Successfuly installed dev headers in $INSTALL_DIR !"
