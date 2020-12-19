#!/bin/bash

################################################################################

function makeAllTestsInDir()
{
    cd $1
    
    rm *.pro.user
    
    FILES=$(ls | grep .pro)

    for FILE in $FILES; do
        NAME=$(basename $FILE .pro)
        DEST="../build-$NAME"
        
        if [[ ! -d $DEST ]]
        then
            mkdir "$DEST"
        fi
        
        cd $DEST
        
        echo -e '\x1b[32;01m'"Building test $NAME..."'\x1b[0m'
        qmake ../$1/$FILE
        make -f $DEST/Makefile qmake_all
        make -j8
        
        cd ../$1
    done
    
    cd ..
}

################################################################################

makeAllTestsInDir ctrl
makeAllTestsInDir main
makeAllTestsInDir models
makeAllTestsInDir utils

################################################################################

read -p "Press any key to continue... " -n1 -s
