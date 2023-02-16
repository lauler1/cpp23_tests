#!/bin/bash

#Try test onlyn: https://godbolt.org/

# -rdynamic is needed for stack trace. It exports the symbols of an executable.

#clang++ -std=c++2b main.cpp -o main.out
clang++ $1 -Wall -rdynamic -std=c++2b main.cpp -I/home/leandro/projects/GSL/include -o main.out

if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


