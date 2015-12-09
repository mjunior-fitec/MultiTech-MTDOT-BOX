#!/bin/bash

DIR=$(pwd)
LIB="libs/"
LIBS=("ISL29011" "MMA845x" "MPL3115A2" "NCP5623B" "GpsParser" "DOGS102")

echo "creating libs directory..."
mkdir -p libs/

for i in ${LIBS[@]}; do
    cd $DIR
    if [ ! -d "$LIB/${i}" ]; then
        echo "cloning ${i}"
        hg clone https://developer.mbed.org/teams/Multi-Hackers/code/${i} $LIB/${i}
    else
        cd $LIB/${i}
        echo "updating ${i}"
        hg pull
    fi
done

echo "generating version header..."
cd $DIR
./version.sh

echo "done"
