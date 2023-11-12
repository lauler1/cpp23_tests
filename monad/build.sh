#!/bin/bash

#Try test onlyn: https://godbolt.org/

#echo $1

# -rdynamic is needed for stack trace. It exports the symbols of an executable.
# -fsanitize=null check for null pointer

#clang++ -std=c++2b main.cpp -o main.out
g++ $1 -ggdb3 -O0 -std=c++11 -Wall -Wextra -pedantic  -Wall -rdynamic -std=c++23 test_optional.cpp -I/home/leandro/projects/GSL/include -I../util/include -o test_optional.out

if [ $? -eq 0 ]; then
	echo "OK: test_optional"
    ./test_optional.out
else
    echo "FAIL: test_optional"
fi
	echo "#########################################################################################################"



g++ $1 -ggdb3 -O0 -std=c++11 -Wall -Wextra -pedantic  -Wall -rdynamic -std=c++23 test_expected.cpp -I/home/leandro/projects/GSL/include -I../util/include -o test_expected.out

if [ $? -eq 0 ]; then
	echo "OK: test_expected"
    ./test_expected.out
else
    echo "FAIL: test_expected"
fi
	echo "#########################################################################################################"



g++ $1 -ggdb3 -O0 -std=c++11 -Wall -Wextra -pedantic  -Wall -rdynamic -std=c++23 test_dynamic.cpp -I/home/leandro/projects/GSL/include -I../util/include -o test_dynamic.out

if [ $? -eq 0 ]; then
	echo "OK: test_dynamic"
    ./test_dynamic.out
else
    echo "FAIL: test_dynamic"
fi



