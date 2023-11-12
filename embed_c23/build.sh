#!/bin/bash

#Try test onlyn: https://godbolt.org/
#To run automatically every 1s:  { while true; do ./build.sh; sleep 1; done; }

#STR1=$(date -r main.cpp)
#
#read STR2 < tmp
#if [ "$STR1" == "$STR2" ]
#then
#
#    exit 0
#fi
#echo -e "${STR1}" > tmp

#------------------------------------------------------------------------

# -rdynamic is needed for stack trace. It exports the symbols of an executable.
# -fsanitize=null check for null pointer
# -Wextra and -Wpedantic generate warning: _Static_assert

#clang++ -std=c++2b main.cpp -o main.out
gcc -std=c2x main.c  -o main.out

#clang++ -std=c++20 -nostdinc++ -nostdlib++ -isystem /home/me/local/include/c++/v1 -isystem /home/me/local/include/x86_64-unknown-linux-gnu/c++/v1 -L /home/me/local/lib/x86_64-unknown-linux-gnu -Wl,-rpath,/home/me/local/lib/x86_64-unknown-linux-gnu -lc++ main.cpp -lc++experimental -o main.out
#-fexperimental-library  
# -L../build/lib
if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


