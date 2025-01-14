#!/bin/bash
RED='\033[0;31m'
GRN='\033[0;32m'
NC='\033[0m' # No Color
CXX="g++"
CXXFLAGS+="-std=c++20 -g -isystem /usr/local/include -iquote ../ -pthread -lgtest -lgtest_main"
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
    if ! ./$OUTPUTFILE 1>> $LOGFILE 2>> $LOGFILE; then
        FAILURE_LIST="$FAILURE_LIST""$TESTNAME "
    else
        echo -e "$GRN$TESTNAME: Success.$NC"

    fi
}

if [ $(basename $(pwd)) != "tests" ]; then
    echo "Error: rerun from the test folder."
    exit 84
fi

# Test suite
runTest "Config/AttackConfig_t.cpp ../src/Config/AttackConfig.cpp" "AttackConfig"
runTest "Config/ImageConfig_t.cpp ../src/Config/ImageConfig.cpp" "ImageConfig"
runTest "Config/EntityConfig_t.cpp ../src/Config/AttackConfig.cpp ../src/Config/EntityConfig.cpp ../src/Config/ImageConfig.cpp" "EntityConfig"
runTest "Config/LevelConfig_t.cpp ../src/Config/LevelConfig.cpp \
    ../src/Config/EntityConfig.cpp ../src/Config/ImageConfig.cpp ../src/Config/AttackConfig.cpp" "LevelConfig"
runTest "Config/AttackConfigResolver_t.cpp ../src/Config/AttackConfig.cpp" "AttackConfigResolver"
runTest "Config/ImageConfig_t.cpp ../src/Config/ImageConfig.cpp" "ImageConfigResolver"
runTest "Config/EntityConfigResolver_t.cpp ../src/Config/EntityConfig.cpp ../src/Config/AttackConfig.cpp ../src/Config/ImageConfig.cpp" "EntityConfigResolver"
runTest "Config/LevelConfigResolver_t.cpp ../src/Config/*.cpp" "LevelConfigResolver"

runTest "Components/Relationship_t.cpp ../src/Components/Relationship.cpp" "Relationship"

#check failure size for print / return status
#FAILURE_LIST=(${FAILURE_LIST// /\n})
if [ "$FAILURE_LIST" != "" ] ; then
    echo -e "Failure(s) (${#FAILURE_LIST[@]}):\tSee the logs and/or bins folders for more info."
    for failure in "${FAILURE_LIST[@]}"
    do
        echo -e "$RED$failure: Failure.$NC"
    done
    exit 84
else
    echo "All success !"
    exit 0
fi

