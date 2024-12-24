#!/bin/bash
CXX="g++"
CXXFLAGS+="-std=c++17 -isystem /usr/local/include -pthread -lgtest -lgtest_main"
FAILURE_LIST=""
##
# @param $1 The file to compile
# @param $2 The name of the test (optional)
# @brief Compile the provided file and run the binary
# @return 84 an error occured
# @return 0 no error encountered
function runTest () {
    if [ "$1" == "" ] ;then
        echo "Error: no file provided."
        return 84
    fi
    TESTNAME="$2"
    if [ "$TESTNAME" == "" ] ;then
        TESTNAME="Unnamed"
        TESTNAME+=$(date +"%H%M%S")
    fi
    OUTPUTFILE="bins/.$TESTNAME.out"
    LOGFILE="logs/$TESTNAME.log"
    echo "$CXX $1 $CXXFLAGS -o $OUTPUTFILE" > $LOGFILE
    if ! $CXX $1 $CXXFLAGS -o $OUTPUTFILE 2>> $LOGFILE ; then
        echo "Error: Cannot compile '$TESTNAME'. See '$LOGFILE'."
        return 84
    fi
    if ! ./$OUTPUTFILE > $LOGFILE; then
        FAILURE_LIST="$FAILURE_LIST""$TESTNAME "
    fi
}

if [ $(basename $(pwd)) != "tests" ]; then
    echo "Error: rerun from the test folder."
    exit 84
fi

# Test suite
runTest "SparseArray_t.cpp" "SparseArray"
runTest "ECS_t.cpp" "ECS"

#check failure size for print / return status
#FAILURE_LIST=(${FAILURE_LIST// /\n})
if [ "$FAILURE_LIST" != "" ] ; then
    echo "Failures (${#FAILURE_LIST[@]}):"
    for failure in "${FAILURE_LIST[@]}"
    do
        echo "- $failure"
    done
    exit 84
else
    echo "All success !"
    exit 0
fi

