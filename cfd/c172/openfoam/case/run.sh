#!/bin/sh

cd ${0%/*} || exit 1

. $WM_PROJECT_DIR/bin/tools/RunFunctions

################################################################################

runApplication surfaceFeatureExtract

################################################################################

runApplication blockMesh

[ ! -d 0 ] && cp -r 0.orig 0

runApplication decomposePar -copyZero
runParallel snappyHexMesh -overwrite

# Mesh preview
# runApplication reconstructParMesh -constant
# exit 1

################################################################################

# runParallel patchSummary ?????
# runParallel potentialFoam ?????
runParallel $(getApplication)
runApplication reconstructParMesh -constant
runApplication reconstructPar -latestTime
# runApplication reconstructPar

runApplication foamLog log.$(getApplication)

################################################################################
