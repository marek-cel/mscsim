#!/bin/bash

################################################################################

TIME_0=$(date +%s)

export EXEC_DIR=$(pwd)

export MODEL_FILE=$EXEC_DIR/models/stl/f16_clean_half.stl


################################################################################

function runCase()
{
    export DEST_DIR=$FOAM_RUN/f16_$1

    cd $EXEC_DIR
    rm -rf $DEST_DIR
    cp -r case $DEST_DIR
    surfaceTransformPoints -rollPitchYaw "( 0 $1 0 )" $MODEL_FILE $DEST_DIR/constant/triSurface/model.stl
    cd $DEST_DIR
    ./run.sh
    cd $FOAM_RUN
    printf "%d\t" $1 &>> forceCoeffs.dat
    tail -n 1 f16_$1/postProcessing/forceCoeffs1/0/forceCoeffs.dat &>> forceCoeffs.dat
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
# runCase 35

################################################################################

TIME_1=$(date +%s)
TIME_T=$(expr $TIME_1 - $TIME_0 )
TIME_H=$(expr $TIME_T / 3600 )
TIME_M=$(expr $(expr $TIME_T - 3600 \* $TIME_H ) / 60 )
TIME_S=$(expr $TIME_T - 3600 \* $TIME_H - 60 \* $TIME_M )
TIME_F=`printf %02d ${TIME_H}`:`printf %02d ${TIME_M}`:`printf %02d ${TIME_S}`

echo "OpenFOAM computations finished in $TIME_F"
notify-send "OpenFOAM computations finished." "OpenFOAM computations finished in $TIME_F"
