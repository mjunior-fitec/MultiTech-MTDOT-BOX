#!/bin/bash

echo "creating libs directory..."
mkdir -p libs/
hg clone https://developer.mbed.org/teams/Multi-Hackers/code/ISL29011/      libs/ISL29011
hg clone https://developer.mbed.org/teams/Multi-Hackers/code/MMA845x/       libs/MMA845x
hg clone https://developer.mbed.org/teams/Multi-Hackers/code/MPL3115A2/     libs/MPL3115A2
hg clone https://developer.mbed.org/teams/Multi-Hackers/code/NCP5623B/      libs/NCP5623B
hg clone https://developer.mbed.org/teams/Multi-Hackers/code/GpsParser/     libs/GpsParser
hg clone https://developer.mbed.org/teams/Multi-Hackers/code/DOGS102/       libs/DOGS102
echo "done"
