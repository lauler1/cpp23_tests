#!/bin/bash

#clang++ -std=c++2b main.cpp -o main.out
clang++ $1 -ggdb3 -O0 -Wall -Wextra -Wpedantic -std=c++20 main.cpp tcpserver.cpp tcpserver_linux.cpp logger.cpp -o main.out

if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


