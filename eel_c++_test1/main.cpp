#include "server.h"
#include <iostream>
#include <cstring>

void test1(Server &server) {
    std::cout << "test1\n";
	auto err = server.start("index.html");
	if(err != 0){
		std::cout << " Error: " << std::strerror(err) << "\n";
		return;
	}
	
	do{
		err = server.run();
	}while(err == 0);
}



int main() {
	
	auto server = Server({.port = 8000, .dir{"web"}});
	
	test1(server);
    
    return 0;
}
