#!/bin/bash
RED='\033[0;31m'
GRN='\033[0;32m'
NC='\033[0m' # No Color
CXX="g++"
CXXFLAGS+="-std=c++20 -isystem /usr/local/include -pthread -lgtest -lgtest_main"
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
        FAILURE_LIST="$FAILURE_LIST""$TESTNAME"
        return 84
    fi
    if ! ./$OUTPUTFILE 1>> $LOGFILE 2>> $LOGFILE; then
        FAILURE_LIST="$FAILURE_LIST""$TESTNAME"
    else
        echo -e "$GRN$TESTNAME: Success.$NC"

    fi
}

if [ $(basename $(pwd)) != "tests" ]; then
    echo "Error: rerun from the test folder."
    exit 84
fi

if [ ! -d "logs" ]; then
    mkdir logs
fi

if [[ $(cat /etc/*-release) == *"Ubuntu"* ]]; then
    sudo apt update
    sudo apt install libgtest-dev -y
fi

if [[ $(cat /etc/*-release) == *"Fedora"* ]]; then
    sudo dnf install gtest-devel -y
fi

# Test suite
runTest "SparseArray_t.cpp" "SparseArray"
runTest "ComponentRegistry_t.cpp" "ComponentRegistry"
runTest "ECS_t.cpp" "ECS"
runTest "Entity_t.cpp" "Entity"
runTest "Resolver/AResolver_t.cpp" "Resolver"
runTest "Graphics/WindowSpecs_t.cpp ../src/Graphics/WindowSpecs.cpp" "WindowSpecs"
runTest "Graphics/UserInputManager_t.cpp ../src/Graphics/UserInputManager.cpp" "UserInputManager"

#check failure size for print / return status
#FAILURE_LIST=(${FAILURE_LIST// /\n})
if [ "$FAILURE_LIST" != "" ] ; then
    echo -e "Failure(s) (${#FAILURE_LIST[@]}):\tSee the logs and/or bins folders for more info."
    for failure in "${FAILURE_LIST[@]}"
    do
        echo -e "$RED$failure: Failure.$NC"
        cat "logs/$failure.log"
    done
    exit 84
else
    echo "All success !"
    exit 0
fi

