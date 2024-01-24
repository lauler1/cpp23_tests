#include "server.h"
#include <iostream>
#include <cstring>
#include <sstream>
#include <iomanip>
#include <ranges>

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
			//response->headers["Sec-WebSocket-Accept"] = get_sec_websocket_accept("q4xkcO32u266gldTuKaSOw==");
			response->headers["Sec-WebSocket-Accept"] = get_sec_websocket_accept(request->headers["Sec-WebSocket-Key"]);

			response->body_message = "";
		}
	}

	return true; //response processed successfully
};

int main() {
	Eel test{};// = new(Test);
	
			/*MimeType mime{};
			std::string mime_type = std::string(mime.get_mime_from_resource("html"));
			std::cout << " MIME type = " << mime_type<< "\n";	
			mime_type = std::string(mime.get_mime_from_resource("jpg"));
			std::cout << " MIME type = " << mime_type<< "\n";	*/
	
	auto server = HttpServer({.port = 8001, .dir{"web"}, .open_browser=false}, test);
	
	test1(server);
    
    return 0;
}
