#!/bin/bash

for fname in $(find . -maxdepth 4 -type f)
    do 
        if [ ${fname##*\.} == "cpp" ];
        then
            echo ${fname}
        fi 
        if [ ${fname##*\.} == "h" ];
        then
            echo ${fname}
        fi 
done | xargs wc -l