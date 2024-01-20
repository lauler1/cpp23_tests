#include "server.h"
#include <iostream>
#include <cstring>

void test1(HttpServer &server) {
    std::cout << "test1\n";
	auto err = server.start("index.html");
	if(err != 0){
		std::cout << " Error: " << std::strerror(err) << "\n";
		return;
	}
	/*
	do{
		err = server.run();
	}while(err == 0);
	
	if(err != 0){
		std::cout << " Error: " << std::strerror(err) << "\n";
		return;
	}
	*/
}



int main() {
	
	auto server = HttpServer({.port = 8001, .dir{"web"}, .open_browser=false});
	
	test1(server);
    
    return 0;
}
