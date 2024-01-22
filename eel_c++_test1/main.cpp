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
	std::string name = "Test Server";

	bool on_request(HttpRequest* request, HttpResponse* response) override;
};

bool Eel::on_request(HttpRequest* request, [[maybe_unused]] HttpResponse* response) {
	std::cout << "###################################################\n";
	std::cout << " on_request ("<<name<<")\n";
	std::cout << "###################################################\n";
	for (const auto& [key, value] : request->headers){
		std::cout << '[' << key << "] = " << value << ";\n";	
	}

	
	bool is_websocket = false;
	if((request->headers.find("Upgrade") != request->headers.end()) and (request->headers["Upgrade"] == "websocket")){
		is_websocket = true;
	}

	if(request->method == "GET"){
		std::cout << "  " << request->method << " resource " << request->resource << "\n";
		if(request->resource.find(".") != std::string::npos){
			
			std::cout << "    of type " << request->resource.substr(request->resource.find(".")) << "\n";
			MimeType mime{};
			std::string mime_type = mime.get_mime_from_resource(request->resource);
			std::cout << " MIME type = " << mime_type<< "\n";
			if(mime_type.find("text/") != std::string::npos){
				response->body_type = BodyType::TXT_FILE;
			}else{
				response->body_type = BodyType::BIN_FILE;
			}
			response->headers["Content-Type"] = mime_type;
			response->body_message = request->resource;

		}else if (is_websocket){
			std::cout << "  Websocket " << request->resource << "\n";
			response->status_code = ResponseCode::SwitchingProtocols;
			response->reason = "Switching Protocols";

			response->headers["Upgrade"] = "websocket";
			response->headers["Connection"] = "Upgrade";
			//response->headers["Sec-WebSocket-Accept"] = get_sec_websocket_accept("q4xkcO32u266gldTuKaSOw==");
			response->headers["Sec-WebSocket-Accept"] = get_sec_websocket_accept(request->headers["Sec-WebSocket-Key"]);

			response->body_message = "";
		}
	}

	return true; //response processed successfully
};

int main() {
	Eel test{};// = new(Test);
	
	auto server = HttpServer({.port = 8001, .dir{"web"}, .open_browser=false}, test);
	
	test1(server);
    
    return 0;
}
