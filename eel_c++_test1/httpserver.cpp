#include "httpserver.h"
#include "sha1.h"

#include <iostream>
#include <fstream>
#include <cerrno>
#include <sstream>
#include <regex>
#include <filesystem>
#include <cassert>

unsigned char buff[] = {
'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};


std::string  my_base64(std::string hex){
	std::string result = "";
	
	bool pading = false;
	
    for (size_t i = 0; i < hex.length(); i += 3) {
        std::string byteString = hex.substr(i, 3);
		if(byteString.length() == 1){
			byteString += "00";
			pading = true;
		}else if(byteString.length() == 2){ // unlikely, missing a 0?
			byteString += "0";
		}
		
		unsigned long val = std::stoul(byteString, nullptr, 16);
		unsigned int low = val & 0x3F;
		unsigned int high = (val >> 6) & 0x3F;
		result += buff[high];
		if(pading){
			result += '=';
		}else{
			result += buff[low];
		}
    }
	return result;
}

std::string get_sec_websocket_accept_attr(std::string key){
	key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	
	SHA1 checksum;
    checksum.update(key);
	std::cout << "  key: "<<key<<"\n";
	std::string hex = checksum.final();
	std::cout << "  SHA1: "<<hex<<"\n";

	std::cout << "  Base64: "<<my_base64(hex)<<"\n";	
	return my_base64(hex);
}

void save_buffer(std::string filename, const char* buffer, size_t len, bool append){
	
	if(buffer == nullptr or len <= 0 or len > 100'000'000 or filename == ""){
		return;
	}
	std::ios_base::openmode mode = std::ios::binary | std::ios::out;
	if(append){
		mode |= std::ios::app;
	}
	std::ofstream outfile;
	outfile.open(filename, mode);
	outfile.write(buffer, len); // sizeof can take a type
	outfile.close();
}

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;
    while (getline (ss, item, delim)) {
        result.push_back (item);
    }
    return result;
}

std::string get_Date(){
	// Time stamp the response with the Date header
	std::string tstr;
	char tbuf[36] = {0};
	time_t rawtime;
	struct tm* ptm;
	time(&rawtime);
	ptm = gmtime(&rawtime);
	// Ex: Fri, 31 Dec 1999 23:59:59 GMT
	strftime(tbuf, 36, "%a, %d %b %Y %H:%M:%S GMT", ptm);
	tstr = tbuf;
	return tstr;
}

std::uintmax_t get_file_size(const std::string &path){
	std::filesystem::path p{path};
	std::uintmax_t ans = std::filesystem::file_size(p);
	std::cout << "     path = " << path <<  "; ans = " << ans << "\n\n";
	return ans;
}

int HttpServer::start(std::string_view start_page){
	if(start_page == ""){
		start_page = conf_.default_page;
	}
	
    std::cout << "HttpServer::start\n";
	std::cout << " start_page: " << start_page << "\n";
    std::cout << "end\n";
	
	pImpl->start_socket();
	
	if(conf_.open_browser){
		open_browser(conf_.dir.append("/").append(start_page));
	}
	
	return 0;//ENOTCONN;
}

bool HttpServer::proc_raw_request(EventData* event_data_ptr){
	assert(event_data_ptr != nullptr);
	
	size_t buff_size = 0;
	char* bin_buff = nullptr;
	event_data_ptr->print("proc_raw_request");
	save_buffer("log.txt", event_data_ptr->buffer, event_data_ptr->length);
	
	/*********************
	Websocket Processing
	**********************/
	if(event_data_ptr->websocket){
		return prepare_ws_response(event_data_ptr);
	}

	/*********************
	Normal HTTP Processing
	**********************/
	HttpRequest request{};
	HttpResponse response{};
	parse_http_request(request, event_data_ptr);

	proc_request_.on_request(&request, &response);
	
	if((response.body_type == BodyType::TXT_FILE) or (response.body_type == BodyType::BIN_FILE)){
		if(!std::filesystem::exists(response.body_message)){
			std::cout << "    FILE DOES NOT EXIST: " << response.body_message << "\n";
			response.status_code = ResponseCode::NotFound;
			response.reason = "Not Found";
			response.body_type = BodyType::TEXT;
			response.body_message ="Not Found";
		}else{
			response.headers["Content-Length"] = std::to_string(get_file_size(response.body_message));
		}
		response.headers["Date"] = get_Date();
	}
	
	if(response.body_type == BodyType::TEXT){
		response.headers["Content-Type"] = "text/plain; charset=utf-8";
		response.headers["Content-Length"] = std::to_string(response.body_message.length());
		response.headers["Date"] = get_Date();
	}
	
	std::string response_string = response.protocol_version+" "+std::to_string((int)response.status_code)+" "+response.reason+"\r\n";
	for (const auto& [key, value] : response.headers){
		response_string += (key+": "+value+"\r\n");
	}
	response_string += "\r\n";
	if(response.body_type == BodyType::TEXT){
		response_string += response.body_message;
	}
	else if(response.body_type == BodyType::TXT_FILE){
		std::ifstream inFile;
		inFile.open(response.body_message); //open the input file
		std::stringstream strStream;
		strStream << inFile.rdbuf();        //read the file
		response_string += strStream.str(); //str holds the content of the file
		inFile.close();
	}
	else if(response.body_type == BodyType::BIN_FILE){

		buff_size = get_file_size(response.body_message);
		bin_buff = new char[buff_size];
		std::ifstream inFile(response.body_message, std::ios::in | std::ios::binary );
		inFile.read(bin_buff, buff_size);
		inFile.close();
	}
	save_buffer("log1.txt", response_string.c_str(), response_string.length());
	std::cout << "\n\nkeep_alive: " << event_data_ptr->keep_alive << "\n\n";
	
	
	return prepare_http_response(response_string, buff_size, bin_buff, event_data_ptr);
}

bool HttpServer::prepare_ws_response(EventData* event_data_ptr){
	WsIncome income{event_data_ptr->buffer, event_data_ptr->get_capacity()};
	WsResponse response{event_data_ptr->buffer, event_data_ptr->get_capacity()};
	std::cout << "\n\n proc_raw_request Websocket Processing\n\n";
	income.id = event_data_ptr->id;
	income.length = event_data_ptr->length;

	income.ws_header.load_from_bytes((unsigned char*)event_data_ptr->buffer);
	income.ws_header.unmask(event_data_ptr->buffer, event_data_ptr->buffer,event_data_ptr->length);

	proc_request_.on_websocket(&income, &response);
	event_data_ptr->length = response.length;
	
	std::cout << "---------------------------------------------------\n";
	std::cout << " Websocket Response\n";
	std::cout << "---------------------------------------------------\n";
	if((income.ws_header.op_code == WebSocketOpCode::Close) or
	(response.ws_header.op_code == WebSocketOpCode::Close)){
		event_data_ptr->keep_alive = false;
		event_data_ptr->length = 0;
		std::cout << "    Will be closed\n\n";
	}else{
		event_data_ptr->buffer[event_data_ptr->length] = 0;
	}
	std::cout << "    Message:\n" << event_data_ptr->buffer << "\n\n";
	std::cout << "    To send " << event_data_ptr->length << " bytes\n";	
	return true;
}

bool HttpServer::prepare_http_response(std::string_view response_string, size_t buff_size, char* bin_buff, EventData *event_data_ptr){
	std::cout << "---------------------------------------------------\n";
	std::cout << " Http Response\n";
	std::cout << "---------------------------------------------------\n";
	std::cout << response_string.data() << "\n\n";

	auto expected_capacity = response_string.length() + buff_size;
	// Need more space?
	if(event_data_ptr->get_capacity() < expected_capacity){
		event_data_ptr->change_capacity(expected_capacity);
	}
	
	// Binary data?
	if(bin_buff != nullptr and buff_size > 0){
		std::memcpy(event_data_ptr->buffer, response_string.data(), response_string.length());
		std::memcpy(&event_data_ptr->buffer[response_string.length()], bin_buff, buff_size);

		event_data_ptr->length = expected_capacity;
		delete[] bin_buff;

	}else{
		std::strcpy(event_data_ptr->buffer, response_string.data());
		event_data_ptr->length = response_string.length();
	}
	return true;
}

bool HttpServer::parse_http_request(HttpRequest &request, EventData *event_data_ptr){
	const std::regex main_header_regex(R"((GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE|PATCH)\s(.+)\s*(HTTP\/\d+\.\d+)*(\r|\n)*)");
	const std::regex attr_header_regex(R"((\S+):\s(.+)(\r|\n)*)");
	const std::regex empty_header_regex(R"((\r|\n)*)"); //Empty line is valid

    std::istringstream istrm{std::string(event_data_ptr->buffer)};
    std::string line;
	request.id = event_data_ptr->id;
	int i = 0;
	while (std::getline(istrm, line)) {
		std::cout << "    " << (i++) << ": " << line << std::endl;
		unsigned long pos;
		if((pos=line.find('\n')) != std::string::npos) line.erase(pos);
		if((pos=line.find('\r')) != std::string::npos) line.erase(pos);
		
		if(std::regex_match(line, main_header_regex)){// Request line
			auto s = split (line, ' '); 
			request.method = s[0];
			request.uri = s[1];
			if(s.size() >= 3) request.protocol_version = s[2]; //may not exist
			
			request.resource = request.uri;
			if(request.resource.find("?")){
					request.resource = request.resource.substr(0, request.resource.find("?"));
			}
			if(request.resource.find("#")){
					request.resource = request.resource.substr(0, request.resource.find("#"));
			}
			if(request.resource == "/"){
				request.resource += conf_.default_page;
			}
			if(request.resource.find(".") != std::string::npos){ // if file, make complete path
				request.resource = conf_.dir+request.resource;
			}else{
				request.resource = request.resource.substr(1); // else, remove path begin
			}
		}
		else if(std::regex_match(line, attr_header_regex)){ // Headers
			auto pos = line.find(": ");
			auto key = line.substr (0,pos); 			
			auto value = line.substr (pos + 2); 			
			request.headers[key] = value;
			
			if(key=="Connection" and value == "keep-alive"){
				event_data_ptr->keep_alive = true;
			}
			if(key=="Upgrade" and value == "websocket"){
				std::cout << "        Upgrade: websocket --> keep_alive \n";
				event_data_ptr->keep_alive = true;
				event_data_ptr->websocket = true;
			}
		}else if(!std::regex_match(line, empty_header_regex)){
			return false;
		}
	}
	return true;	
};

std::string_view test_request1 = R"(GET /websocket.html#:~:text=In?v=HgjUmDTMywo HTTP/1.1
Connection: keep-alive
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36
Accept-Language: en-GB,en;q=0.9,de-DE;q=0.8,de;q=0.7,pt-BR;q=0.6,pt;q=0.5,en-US;q=0.4

)";

std::string_view test_request2 = R"(LET /websocket.html HTTP/1.1
Connection: keep-alive

)";

std::string_view test_response1 = R"(HTTP/1.1 200 OK
Content-Length: 0
Content-Type: text/html
Date: Fri, 26 Jan 2024 16:59:45 GMT

)";

// Unit Test of the Server Message processing
void HttpServer::internal_unittests(){
	
	// test Mime
	MimeType mime{};
	assert(mime.get_mime_from_resource("html") == "text/html");
	assert(mime.get_mime_from_resource("json") == "application/json");

	// parse_http_request: parse a valid http header -> return true
	{
	EventData event_data(const_cast<char *>(test_request1.data()), test_request1.length());
	HttpRequest request{};
	assert(parse_http_request(request, &event_data));
	assert(request.method == "GET");
	assert(request.uri =="/websocket.html#:~:text=In?v=HgjUmDTMywo");
	assert(request.resource == (conf_.dir+"/websocket.html"));
	assert(request.protocol_version == "HTTP/1.1");	
	assert(request.headers["Connection"] == "keep-alive");
	assert(request.headers["User-Agent"] == "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
	assert(request.headers["Accept-Language"] == "en-GB,en;q=0.9,de-DE;q=0.8,de;q=0.7,pt-BR;q=0.6,pt;q=0.5,en-US;q=0.4");
	event_data.buffer = nullptr; // test buffer is read only, cannot be deleted
	}
	
	// parse a invalid http header -> return false
	{
	EventData event_data(const_cast<char *>(test_request2.data()), test_request2.length());
	HttpRequest request{};
	assert(!parse_http_request(request, &event_data));
	event_data.buffer = nullptr; // test buffer is read only, cannot be deleted
	}
	
	// prepare_ws_response:
	// parse a valid Websocket packet -> return true
	// parse a invalid Websocket packet -> return false

	// prepare_http_response: <- Must be refactored
	// Text only -> out buffer contains data
	{
	auto event_data = new EventData();// <- Must use new
	prepare_http_response(test_response1, 0, nullptr, event_data);
	assert(event_data->length == (ssize_t)test_response1.length());
	assert(std::strcmp(event_data->buffer, test_response1.data()) == 0);
	delete event_data;
	}

	// Text only, Need more space -> out buffer is re-allocated and contains data
	{
	auto event_data = new EventData(10);
	prepare_http_response(test_response1, 0, nullptr, event_data);
	assert(event_data->length == (ssize_t)test_response1.length());
	assert(event_data->get_capacity() == test_response1.length());
	assert(std::strcmp(event_data->buffer, test_response1.data()) == 0);
	delete event_data;
	}
	
	// Binary -> out buffer contains header + in buffer data
	{
	auto event_data = new EventData();
	char *buff = new char[3]; buff[0] = 'H'; buff[1] = 'i'; buff[2] = '!'; // <- Must use new
	char buff2[3]; memcpy(buff2, buff, 3); // <- because buff will be deleted by prepare_http_response
	prepare_http_response(test_response1, 3, &buff[0], event_data);
	assert(event_data->length == (ssize_t)(test_response1.length()+(size_t)3));
	//assert(event_data->get_capacity() == (test_response1.length()+3));
	std::string new_str = std::string(test_response1) + std::string (std::string_view(buff2, 3));
	assert(std::strcmp(std::string_view(event_data->buffer, event_data->length).data(), new_str.c_str()) == 0);
	delete event_data;
	}
	
	// Binary, Need more space -> out buffer is re-allocated and contains header + in buffer data
	{
	auto event_data = new EventData(1);
	char *buff = new char[3]; buff[0] = 'H'; buff[1] = 'i'; buff[2] = '!'; // <- Must use new
	char buff2[3]; memcpy(buff2, buff, 3); // <- because buff will be deleted by prepare_http_response
	prepare_http_response(test_response1, 3, &buff[0], event_data);
	assert(event_data->length == (ssize_t)(test_response1.length()+(size_t)3));
	std::string new_str = std::string(test_response1) + std::string (std::string_view(buff2, 3));
	assert(event_data->get_capacity() == (test_response1.length()+3));
	assert(std::strcmp(std::string_view(event_data->buffer, event_data->length).data(), new_str.c_str()) == 0);
	delete event_data;
	}
	
	std::cout << "Success: All tests passed.\n";
}
