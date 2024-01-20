#include "server.h"

#include <iostream>
#include <cerrno>


/**
    std::cout << "t << std::endl;
}

std::function<void()> decorator(std::function<void()> func) {
    return [&]() {
        std::cout << "Decorator start" << std::endl;
        func();
        std::cout << "Decorator end" << std::endl;
    };
}

int main() {
	
	// Example 1, using simpleFunction
    auto decorated = decorator(simpleFunction);
    decorated();
	
	std::cout << "\n";

	// Example 2, using lambda
    auto decoratedLambda = decorator([]() {
        std::cout << "Inside lambda function!" << std::endl;
    });
    decoratedLambda();
    
    return 0;
}
*/
