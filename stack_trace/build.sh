#!/bin/bash

#Try test onlyn: https://godbolt.org/

#echo $1

#clang++ -std=c++2b main.cpp -o main.out
clang++ $1 -std=c++2b main.cpp -I/home/leandro/projects/GSL/include -o main.out

if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


