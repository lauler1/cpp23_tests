#include "server.h"
#include <iostream>
#include <cerrno>

extern int open_browser(std::string url);

Server::Server(ServInit params){
	conf = params;
    std::cout << "Server::Server" << std::endl;
    std::cout << " conf.port: " << conf.port << "\n";
    std::cout << " conf.dir: "  << conf.dir << "\n";
    std::cout << "end" << std::endl;
}

int Server::start(std::string_view start_page){
	
    std::cout << "Server::start" << std::endl;
	std::cout << " start_page: " << start_page << "\n";
    std::cout << "end" << std::endl;
	
	open_browser(conf.dir.append("/").append(start_page));
	
	return 0;//ENOTCONN;
}

int Server::run(){
    std::cout << "Server::run" << std::endl;


    std::cout << "end" << std::endl;	
	return 1;
}

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
