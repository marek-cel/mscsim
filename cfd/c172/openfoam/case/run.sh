#!/bin/bash

################################################################################

function getApplication()
{
    foamDictionary -entry application -value system/controlDict
}

getNumberOfProcessors()
{
    foamDictionary -entry numberOfSubdomains -value system/decomposeParDict
}

################################################################################

echo "Running surfaceFeatureExtract"
surfaceFeatureExtract > log.surfaceFeatureExtract 2>&1

################################################################################

echo "Running blockMesh"
blockMesh > log.blockMesh 2>&1

echo "Copying 0.orig to 0"
cp -r 0.orig 0

echo "Running decomposePar"
decomposePar -copyZero > log.decomposePar 2>&1

echo "Running snappyHexMesh in parallel using $(getNumberOfProcessors) processes"
mpirun -np $(getNumberOfProcessors) snappyHexMesh -parallel -overwrite > log.snappyHexMesh 2>&1

# Mesh preview
# echo "Running reconstructParMesh"
# reconstructParMesh -constant

################################################################################

# echo "Running patchSummary in parallel using $(getNumberOfProcessors) processes"
# mpirun -np $(getNumberOfProcessors) patchSummary -parallel > log.patchSummary 2>&1

# echo "Running potentialFoam in parallel using $(getNumberOfProcessors) processes"
# mpirun -np $(getNumberOfProcessors) potentialFoam -parallel > log.potentialFoam 2>&1

echo "Running $(getApplication) in parallel using $(getNumberOfProcessors) processes"
mpirun -np $(getNumberOfProcessors) $(getApplication) -parallel > log.$(getApplication) 2>&1

echo "Running reconstructParMesh"
reconstructParMesh -constant > log.reconstructParMesh 2>&1

echo "Running reconstructPar"
reconstructPar -latestTime > log.reconstructPar 2>&1
# reconstructPar > log.reconstructPar 2>&1

echo "Running foamLog"
foamLog log.$(getApplication)

################################################################################
