#!/bin/bash

if ! cpplint --version 1> /dev/null; then
    echo "Error: cpplint not installed."
    echo "Refer to https://github.com/cpplint/cpplint?tab=readme-ov-file to install it"
    exit 84
fi
LOG_FILE="style_error.log"
OPTION="--recursive --linelength=160"
SRC_DIR="src/"
TMP=.tmp

eval cpplint $OPTION $SRC_DIR 2> $LOG_FILE 1> $TMP
if ! [ -s $LOG_FILE ]; then
    echo "No error detected !"
    exit 0
fi

cat $LOG_FILE >&2
echo $(eval cat $TMP | grep "Total error")
echo "Errors are registered in $LOG_FILE."
