#!/bin/bash

DIR=$(pwd)
LIB="libs/"

# 2 parameters
# param1: string name of library
# param2: number mercurial revision to use
update_hg_lib() {
    cd $LIB

    if [ ! -d "$1" ]; then
        echo "cloning $1"
        hg clone https://developer.mbed.org/teams/Multi-Hackers/code/$1 ./$1
    fi

    cd $1
    echo "checkout revision $2 of $1"
    hg checkout $2
    
    cd $DIR
}

echo "update submodules"

git submodule update --recursive --init

mdot-library/version.sh
./version.sh

echo "creating libs directory..."
mkdir -p $LIB

update_hg_lib "ISL29011" 3
update_hg_lib "MMA845x" 3
update_hg_lib "MPL3115A2" 2
update_hg_lib "NCP5623B" 3
update_hg_lib "GpsParser" 10
update_hg_lib "DOGS102" 4

echo "generating version header..."
cd $DIR
./version.sh

echo "done"
