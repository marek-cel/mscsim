#!/bin/bash

################################################################################

EXECDIR=$(pwd)

function run()
{
    export DESTDIR=$FOAM_RUN/c172_$1

    cd $EXECDIR
    rm -rf $DESTDIR
    cp -r case $DESTDIR
    osgconv -o $1-0,1,0 models/stl/c172.stl $DESTDIR/constant/triSurface/model.stl
    cd $DESTDIR
    ./run.sh
}

################################################################################

run 10

# for i in {-180..-30..10}
# do
#    run $i
# done
# 
# for i in -25 -20 -17 -14 -12
# do
#    run $i
# done

# for i in {-10..10..1}
# do
#    run $i
# done

# for i in 12 14 17 20 25
# do
#    run $i
# done
# 
# for i in {30..180..10}
# do
#    run $i
# done

################################################################################
