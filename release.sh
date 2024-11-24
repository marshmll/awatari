#!/usr/bin/bash

if [ ! -d "Release/" ]
then
    mkdir Release
    cd Release/
    cmake -DCMAKE_BUILD_TYPE=Release ..
    cd ../
fi

clear
cd Release/ &&
make &&
cd bin/ &&
clear &&
./awatari &&
cd ../../
