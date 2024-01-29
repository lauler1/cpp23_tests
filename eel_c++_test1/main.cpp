#include "httpserver.h"
#include "tcpserver.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <ranges>
#include <cassert>

/** Full server */
/*
void test1(HttpServer &server) {

	
	server.internal_unittests();

	// Receies a packet not http neither Websocket -> automatic 500 BadRequest
	//			websocket  = false
	//			keep_alive = false

	// proc_raw_request:
	// Receives a simple GET with text -> valid 200 OK mewssage
	//			websocket  = false
	//			keep_alive = false

	// Receives a simple GET with text keep alive -> valid 200 OK mewssage
	//			websocket  = false
	//			keep_alive = true

	// Receives a request to upgrade to Websocket -> valid 200 OK mewssage:
	//			websocket  = true
	//			keep_alive = true

	// Receives a Websocket packet with imasks -> valid
	//			websocket  = true
	//			keep_alive = true
	
}


// Full server
void test2(HttpServer &server) {
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
*/
struct Eel: Request_Callback_Interface{
	std::string name = "Test Server";

	bool on_request(HttpRequest* request, HttpResponse* response) override;
	bool on_websocket(WsIncome* income, WsResponse* response) override;
};

std::string bufferToHexString(const unsigned char* buffer, size_t length) {
    std::stringstream hexString;
    for (size_t i = 0; i < length; ++i) {
        hexString << std::hex << std::setw(2) << std::setfill('0') << (int)buffer[i];
    }
    return hexString.str();
}

bool Eel::on_websocket([[maybe_unused]] WsIncome* income, [[maybe_unused]] WsResponse* response) {
	std::cout << "###################################################\n";
	std::cout << " on_websocket ("<<name<<") id = "<< income->id <<"\n";
	std::cout << "###################################################\n";
	std::cout << "    Received " << income->length << " bytes\n";
	std::cout << "     income WebSocketHeader:\n" << income->ws_header.to_string() << "\n\n";
	std::cout << bufferToHexString((unsigned char*)income->buff_ptr, income->length) << "\n\n";
	std::cout << "     Message:\n" << income->buff_ptr << "\n\n";
	
	
	char * payload_ptr = response->buff_ptr+2;
	size_t len = sprintf(payload_ptr, "Answer from the Server: %s, id=%lu.", name.c_str(), income->id);
	std::cout << "    response WebSocketHeader:\n" << response->ws_header.to_string() << "\n\n";
	std::cout << "     to send: " << len << " bytes of payload\n";
	std::cout << "     message: " << payload_ptr << "\n\n";
	response->ws_header.set_len(len);
	response->length = len+2;
	response->ws_header.write_to_buff((unsigned char *)response->buff_ptr);
	std::cout << "     response WebSocketHeader:\n      " << response->ws_header.to_string() << "\n\n";
	return true; //response processed successfully
}

bool Eel::on_request(HttpRequest* request, [[maybe_unused]] HttpResponse* response) {
	std::cout << "###################################################\n";
	std::cout << " on_request ("<<name<<") id = "<< request->id <<"\n";
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
			//get only the extension e.g. "bla/blabla.html" -> "html"
			auto extension = request->resource.substr(request->resource.find("."));
			if(extension.size() > 1) extension = extension.substr(1);
			
			std::cout << "    of type " << request->resource << "\n";
			MimeType mime{};
			
			std::string mime_type = std::string(mime.get_mime_from_resource(extension));
			std::cout << "    MIME type = " << mime_type<< "\n";
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
			response->headers["Sec-WebSocket-Accept"] = get_sec_websocket_accept_attr(request->headers["Sec-WebSocket-Key"]);

			response->body_message = "";
		}
	}

	return true; //response processed successfully
};


struct HttpDecorator: ServerDecoratorInterface{
	std::string name = "Test Server";

	bool proc_raw_request(EventData* event_data_ptr){
		std::cout << "HttpDecorator::proc_raw_request\n";
		event_data_ptr->length = 0; // <- don't send data
		return true;
	}
};


int main() {
	
	Eel test{};
	
	auto httpserver = HttpServer({.dir{"web"}}, test);
	HttpDecorator http_decorator{};
	auto server = TcpServer(8001, httpserver);
	auto err = server.start();
	if(err != ServError::NO_ERROR){
		std::cout << " server.start error\n";
		return -1;
	}

	do{
		err = server.run();
	}while(err == ServError::NO_ERROR);
	
	//test2(server);
    
    return 0;
}
