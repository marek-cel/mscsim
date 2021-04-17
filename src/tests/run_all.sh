#!/bin/bash

################################################################################

function runAllTestsInDir()
{
    cd $1
    
    FILES=$(ls | grep .pro)

    for FILE in $FILES; do
        cd ..
        NAME=$(basename $FILE .pro)
        DEST="temp_dest_dir"
        
        cd $DEST
        
        ./$NAME > "$NAME.log"
        CODE=$?
        
        if [[ $CODE -eq 0 ]]
        then
            echo -e '\x1b[32;01m'"Test $NAME succeeded."'\x1b[0m'
        else
            echo -e '\x1b[31;01m'"Test $NAME failed. Exit code: $CODE"'\x1b[0m'
            read -p "Press any key to continue... " -n1 -s
            exit
        fi
        
        cd ../$1
    done
    
    cd ..
}

################################################################################

runAllTestsInDir ctrl
runAllTestsInDir main
# runAllTestsInDir models
runAllTestsInDir utils

################################################################################

echo -e '\x1b[32;01m'"ALL TESTS SYCCEEDED."'\x1b[0m'
echo

################################################################################
