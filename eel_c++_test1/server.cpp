#include "server.h"
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
	
	//std::cout << "  my_base64: hex = "<<hex<<"\n";
	
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
		
		//std::cout << "  my_base64: 0x"<<byteString<<" = "<<val<<std::endl;
		
		unsigned int low = val & 0x3F;
		unsigned int high = (val >> 6) & 0x3F;
		/*if(low > 63 or high > 63){
			//std::cout << "  my_base64: values > 63\n";
		}*/
		result += buff[high];
		if(pading){
			result += '=';
		}else{
			result += buff[low];
		}
		//std::cout << "  my_base64: low = "<<low<<", high = "<<high<< ", high = "<< buff[high] << ", low="<< buff[low] <<std::endl;
    }//					  40	101000	o
	//std::cout << "  my_base64: result = "<<result<<std::endl;
	return result;
}

std::string get_sec_websocket_accept(std::string key){
	
	//const char[] = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
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

std::tuple<std::string, size_t, char*> HttpServer::proc_raw_request(EventData* event_data_ptr){
	assert(event_data_ptr != nullptr);
	
	size_t buff_size = 0;
	char* bin_buff = nullptr;
	event_data_ptr->print("proc_raw_request");

	save_buffer("log.txt", event_data_ptr->buffer, event_data_ptr->length);
	
	/*********************
	Websocket Processing
	**********************/
	if(event_data_ptr->websocket){
		WsIncome income{event_data_ptr->buffer, event_data_ptr->get_capacity()};
		WsResponse response{event_data_ptr->buffer, event_data_ptr->get_capacity()};
		std::cout << "\n\n proc_raw_request Websocket Processing\n\n";
		income.id = event_data_ptr->id;
		income.length = event_data_ptr->length;

		income.ws_header.load_from_bytes((unsigned char*)event_data_ptr->buffer);
		income.ws_header.unmask(event_data_ptr->buffer, event_data_ptr->buffer,event_data_ptr->length);

		proc_request_.on_websocket(&income, &response);

		event_data_ptr->length = response.length;

		return std::make_tuple("", event_data_ptr->length, event_data_ptr->buffer);
	}

	/*********************
	Normal HTTP Processing
	**********************/
    std::istringstream istrm{std::string(event_data_ptr->buffer)};
    std::string line;
	HttpRequest request{};
	HttpResponse response{};
	const std::regex main_header_regex(R"((GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE|PATCH)\s(.+)\s*(HTTP\/\d+\.\d+)*(\r|\n)*)");
	const std::regex attr_header_regex(R"((\S+):\s(.+)(\r|\n)*)");
	std::cout << "\n\n proc_raw_request Normal HTTP Processing:\n" << event_data_ptr->buffer << "\n\n";
	std::cout << "\n\nkeep_alive: " << event_data_ptr->keep_alive << "\n\n";


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
			std::cout << "        method="<<request.method<<", uri="<<request.uri<<"\n";
			if(s.size() >= 3) request.protocol_version = s[2]; //may not exist
			std::cout << "        protocol_version="<<request.protocol_version<<"\n";
			
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
			std::cout << "        resource="<<request.resource<<"\n";
		}
		else if(std::regex_match(line, attr_header_regex)){ // Headers
			//auto s = split (line, ':');
			auto pos = line.find(": ");
			auto key = line.substr (0,pos); 			
			auto value = line.substr (pos + 2); 			
			std::cout << "        key="<<key<<", value="<<value<<"\n";
			request.headers[key] = value;
			
			if(key=="Connection" and value == "keep-alive"){
				event_data_ptr->keep_alive = true;
			}
			if(key=="Upgrade" and value == "websocket"){
				std::cout << "        Upgrade: websocket --> keep_alive \n";
				event_data_ptr->keep_alive = true;
				event_data_ptr->websocket = true;
			}
		}
		else{ // TODO (something else, e.g. user raw data)
			//std::cout << "\n proc_raw_request:\n" << event_data_ptr->buffer << "\n\n";
			//std::cout << "    Http header error on line " << (i++) << ": " << line << std::endl;
		}
	}

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
		strStream << inFile.rdbuf(); //read the file
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
	return std::make_tuple(response_string, buff_size, bin_buff);
}
