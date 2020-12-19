#!/bin/bash

rm -R 

################################################################################

function cleanAllTestInDir()
{
    cd $1
    
    rm *.pro.user
    
    FILES=$(ls | grep .pro)

    for FILE in $FILES; do
        NAME=$(basename $FILE .pro)
        DEST="../build-$NAME"
        
        rm -R $DEST
    done
    
    cd ..
}

################################################################################

cleanAllTestInDir ctrl
cleanAllTestInDir main
cleanAllTestInDir models
cleanAllTestInDir utils

################################################################################

read -p "Press any key to continue... " -n1 -s
