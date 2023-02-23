#!/bin/bash


clang++ -std=c++20 -x c++-module hello_test.cpp --precompile -o hello_test.pcm
clang++ -std=c++20 main.cpp -fprebuilt-module-path=. hello_test.pcm -o main.out

if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


