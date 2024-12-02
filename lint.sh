#!/bin/bash
# 2024 Bottiglione Julian <julian.bottiglione@epitech.eu>

if ! cpplint --version 1> /dev/null; then
    echo "Error: cpplint not installed."
    echo "Refer to https://github.com/cpplint/cpplint?tab=readme-ov-file to install it"
    exit 84
fi
LOG_FILE="style_error.log"
OPTION="--recursive"
SRC_DIR="src/"
FILTER="--filter=-build/include_order,-whitespace/braces,-whitespace/line_length,-legal/copyright,-build/c++11,-whitespace/indent,-whitespace/parens,-runtime/references"
TMP=.tmp

eval cpplint $OPTION $FILTER $SRC_DIR 2> $LOG_FILE 1> $TMP
if ! [ -s $LOG_FILE ]; then
    echo "No error detected !"
    rm $TMP || true > /dev/null
    exit 0
fi

cat $LOG_FILE >&2
echo $(eval cat $TMP | grep "Total error")
echo "Errors are registered in $LOG_FILE."
rm $TMP || true > /dev/null
