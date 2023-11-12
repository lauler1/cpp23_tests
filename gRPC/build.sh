#!/bin/bash

#Try test onlyn: https://godbolt.org/

#echo $1

# -rdynamic is needed for stack trace. It exports the symbols of an executable.
# -fsanitize=null check for null pointer
# -Wextra and -Wpedantic generate warning: _Static_assert

#clang++ -std=c++2b main.cpp -o main.out
g++ -std=c++11 math_service.pb.cc math_service.grpc.pb.cc math_server.cc -o math_server `pkg-config --cflags --libs protobuf grpc++`

g++ -std=c++11 math_service.pb.cc math_service.grpc.pb.cc math_client.cc -o math_client `pkg-config --cflags --libs protobuf grpc++`


# -L../build/lib
if [ $? -eq 0 ]; then
	echo "OK"
    ./main.out
else
    echo "FAIL"
fi


