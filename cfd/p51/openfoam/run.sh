#!/bin/bash

################################################################################

TIME_0=$(date +%s)
echo $(date +%Y-%m-%d\ %H:%M:%S) - Computations started

export EXEC_DIR=$(pwd)

################################################################################

# export MODEL_NAME=p51_half
# export MODEL_FILE=$EXEC_DIR/models/stl/$MODEL_NAME.stl
# ./iterateAngleOfAttack.sh

# cd $EXEC_DIR
# export MODEL_NAME=p51_tail_off_half
# export MODEL_FILE=$EXEC_DIR/models/stl/$MODEL_NAME.stl
# ./iterateAngleOfAttack.sh

cd $EXEC_DIR
export MODEL_NAME=p51_flaps_30_half
export MODEL_FILE=$EXEC_DIR/models/stl/$MODEL_NAME.stl
./iterateAngleOfAttack.sh

# cd $EXEC_DIR
# export MODEL_NAME=p51_elevator_10_half
# export MODEL_FILE=$EXEC_DIR/models/stl/$MODEL_NAME.stl
# ./iterateAngleOfAttack.sh

################################################################################

# cd $EXEC_DIR
# export MODEL_NAME=p51
# export MODEL_FILE=$EXEC_DIR/models/stl/$MODEL_NAME.stl
# ./iterateSideslipAngle.sh
# 
# cd $EXEC_DIR
# export MODEL_NAME=p51_tail_off
# export MODEL_FILE=$EXEC_DIR/models/stl/$MODEL_NAME.stl
# ./iterateSideslipAngle.sh

################################################################################

TIME_1=$(date +%s)
TIME_T=$(expr $TIME_1 - $TIME_0 )
TIME_H=$(expr $TIME_T / 3600 )
TIME_M=$(expr $(expr $TIME_T - 3600 \* $TIME_H ) / 60 )
TIME_S=$(expr $TIME_T - 3600 \* $TIME_H - 60 \* $TIME_M )
TIME_F=`printf %02d ${TIME_H}`:`printf %02d ${TIME_M}`:`printf %02d ${TIME_S}`

echo "OpenFOAM computations finished in $TIME_F"
notify-send "OpenFOAM computations finished." "OpenFOAM computations finished in $TIME_F"
