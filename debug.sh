#!/usr/bin/bash

if [ ! -d "Debug/" ]
then
    mkdir Debug
    cd Debug/
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cd ../
fi

clear
cd Debug/ &&
make &&
cd bin/ &&
clear &&
./awatari &&
cd ../../
