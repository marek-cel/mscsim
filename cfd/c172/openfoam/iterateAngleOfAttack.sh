#!/bin/bash

################################################################################

export EXEC_DIR=$(pwd)

################################################################################

function runCase()
{
    export DEST_DIR="$FOAM_RUN/$MODEL_NAME""_$1"

    cd $EXEC_DIR
    rm -rf $DEST_DIR
    cp -r case $DEST_DIR
    surfaceTransformPoints -rollPitchYaw "( 0 $1 0 )" $MODEL_FILE $DEST_DIR/constant/triSurface/model.stl
    cd $DEST_DIR
    ./run.sh
    cd $FOAM_RUN
    printf "%d\t" $1 &>> forceCoeffs_$MODEL_NAME.dat
    tail -n 1 "$MODEL_NAME""_$1"/postProcessing/forceCoeffs1/0/forceCoeffs.dat &>> forceCoeffs_$MODEL_NAME.dat
}

################################################################################

function run()
{
    for i in {-180..-30..10}
    do
      runCase $i
    done

    for i in -25 -20 -17 -14 -12
    do
      runCase $i
    done

    for i in {-10..15..1}
    do
      runCase $i
    done

    for i in 17 20 25
    do
      runCase $i
    done

    for i in {30..180..10}
    do
      runCase $i
    done
}

################################################################################

run

################################################################################
