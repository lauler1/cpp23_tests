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

	do{
		err = server.run();
	}while(err == 0);
	
	if(err != 0){
		std::cout << " Error: " << std::strerror(err) << "\n";
		return;
	}

}

struct Eel: Request_Callback_Interface{
	std::string name = "Nome...";

	void proc_request(HttpRequest* request, HttpResponse* response) override;
};

void Eel::proc_request(HttpRequest* request, [[maybe_unused]] HttpResponse* response) {
	std::cout << "###################################################\n";
	std::cout << " proc_request "<<name<<"\n";
	std::cout << "###################################################\n";
	for (const auto& [key, value] : request->headers){
		std::cout << '[' << key << "] = " << value << ";\n";	
	}
	
	
	response->headers["Connection"] = "close";
	response->headers["Content-Type"] = "text/html; charset=utf-8";
	
	/**
	if body_type = BodyType::NONE, body_message is ignored
	if body_type = BodyType::TEXT, body_message is text sent  
	if body_type = BodyType::TXT_FILE, body_message is the file name to be load (For text file)
	if body_type = BodyType::BIN_FILE, body_message is the file name to be load (For binary file)
	*/
	response->body_type = BodyType::TXT_FILE;
	response->body_message = request->resource;

	
};

int main() {
	Eel test{};// = new(Test);
	
	auto server = HttpServer({.port = 8001, .dir{"web"}, .open_browser=false}, test);
	
	test1(server);
    
    return 0;
}
