#!/bin/bash

################################################################################

function makeAllTestsInDir()
{
    cd $1
    
    rm *.pro.user
    
    FILES=$(ls | grep .pro)

    for FILE in $FILES; do
        cd ..
        NAME=$(basename $FILE .pro)
        DEST="temp_build_dir/build-$NAME"
        
        if [[ ! -d $DEST ]]
        then
            mkdir "$DEST"
        fi
        
        cd $DEST
        
        echo -e '\x1b[32;01m'"Building test $NAME..."'\x1b[0m'
        qmake ../../$1/$FILE
        make -f Makefile qmake_all
        make -j8
        CODE=$?
        
        if [[ $CODE -eq 0 ]]
        then
            echo -e '\x1b[32;01m'"Test $NAME succeeded."'\x1b[0m'
        else
            echo -e '\x1b[31;01m'"Test $NAME failed. Exit code: $CODE"'\x1b[0m'
            read -p "Press any key to continue... " -n1 -s
            exit
        fi
        
        cp $NAME ../../temp_dest_dir
        
        cd ../../$1
    done
    
    cd ..
}

################################################################################

rm -R temp_build_dir
rm -R temp_dest_dir

mkdir temp_build_dir
mkdir temp_dest_dir

################################################################################

makeAllTestsInDir ctrl
makeAllTestsInDir main
makeAllTestsInDir models
makeAllTestsInDir utils

################################################################################

echo

################################################################################
