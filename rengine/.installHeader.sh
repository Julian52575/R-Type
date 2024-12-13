#!/bin/bash
# 
HPP_LIST=$(find . -name "*.hpp" -not -path "./lol/*.hpp" -not -path "./build/*.hpp" -not -path "./rengine/*.hpp" | tr " " "\n")
HPP_LIST=(${HPP_LIST// /\n})
INSTALL_PARENT_DIR=/usr/include
INSTALL_DIR="$INSTALL_PARENT_DIR/rengine"

# Rights check
TEST_FILE="rengine.test"
touch $TEST_FILE
if ! mv $TEST_FILE $INSTALL_PARENT_DIR; then
    echo "WARNING: Cannot copy dev headers into $INSTALL_DIR."
    INSTALL_DIR="$(pwd)/rengine"
    echo "Installing in $INSTALL_DIR instead."
    echo "Copy $INSTALL_DIR into $INSTALL_DIR yourself."
fi
rm $TEST_FILE 2> /dev/null || true
rm "$INSTALL_PARENT_DIR/$TEST_FILE" 2> /dev/null || true

# Clear target directory
rm -rf $INSTALL_DIR 2> /dev/null || true
for header in "${HPP_LIST[@]}"
do
    mkdir -p "$INSTALL_DIR/$(dirname $header)"
    cp -a $header "$INSTALL_DIR/$header"
done
echo "Installed dev headers in $INSTALL_DIR !"
if [ "$INSTALL_DIR" != "/usr/include/rengine" ]; then
    echo "WARNING: Move $INSTALL_DIR to /usr/include/rengine"
fi
