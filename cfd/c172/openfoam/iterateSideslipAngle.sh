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
    rm -f $DEST_DIR/system/forceCoeffs
    cp case_sideslip/system/forceCoeffs system/forceCoeffs
    surfaceTransformPoints -rollPitchYaw "( 0 0 $1 )" $MODEL_FILE $DEST_DIR/constant/triSurface/model.stl
    cd $DEST_DIR
    ./run.sh
    cd $FOAM_RUN
    printf "%d\t" $1 &>> forceCoeffs_$MODEL_NAME.dat
    tail -n 1 "$MODEL_NAME""_$1"/postProcessing/forceCoeffs1/0/forceCoeffs.dat &>> forceCoeffs_$MODEL_NAME.dat
}

################################################################################

function run()
{
    for i in -90 -60 -30 -20 -10 -5  0  5  10  20  30  60  90
    do
      runCase $i
    done
}

################################################################################

run 

################################################################################
