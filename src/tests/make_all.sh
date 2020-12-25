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
