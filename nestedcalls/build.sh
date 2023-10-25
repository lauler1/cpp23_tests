#!/bin/bash

#Try test onlyn: https://godbolt.org/

#echo $1

# -rdynamic is needed for stack trace. It exports the symbols of an executable.
# -fsanitize=null check for null pointer
# -Wextra and -Wpedantic generate warning: _Static_assert

clang++ -std=c++2b main.cpp -o main.out
#clang++ $1 -ggdb3 -O0 -Wall -Wextra -Wpedantic -rdynamic -std=c++2b main.cpp -I/home/leandro/projects/GSL/include -I../util/include -lgtest -o main.out
# -L../build/lib
if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


