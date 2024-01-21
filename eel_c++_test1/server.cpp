#include "server.h"

#include <iostream>
#include <fstream>
#include <cerrno>
#include <sstream>
#include <regex>
#include <filesystem>

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
	return std::filesystem::file_size(p);
}

std::optional<std::string> HttpServer::proc_raw_request(EventData* event_data_ptr){
	HttpRequest request;
	HttpResponse response;
	
	std::cout << "\n\ndo_something:\n" << event_data_ptr->buffer << "\n\n";
    std::istringstream f{std::string(event_data_ptr->buffer)};
    std::string line;
	const std::regex main_header_regex(R"((GET|HEAD|POST|PUT|DELETE|CONNECT|OPTIONS|TRACE|PATCH)\s(.+)\s*(HTTP\/\d+\.\d+)*(\r|\n)*)");
	const std::regex attr_header_regex(R"((\S+):\s(.+)(\r|\n)*)");

	int i = 0;
    while (std::getline(f, line)) {
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
			request.resource = conf_.dir+request.resource;
			
			std::cout << "        resource="<<request.resource<<"\n";

		}
		else if(std::regex_match(line, attr_header_regex)){ // Headers
			//auto s = split (line, ':');
			auto pos = line.find(": ");
			auto key = line.substr (0,pos); 			
			auto value = line.substr (pos + 2); 			
			std::cout << "        key="<<key<<", value="<<value<<"\n";
			request.headers[key] = value;
		}
		else{ // TODO (something else, e.g. user raw data)
			//std::cout << "\n proc_raw_request:\n" << event_data_ptr->buffer << "\n\n";
			std::cout << "    Http header error on line " << (i++) << ": " << line << std::endl;
		}
    }	
	
	response.headers["Date"] = get_Date();
	
	proc_request_.proc_request(&request, &response);
	
	if((response.body_type == BodyType::TXT_FILE) or (response.body_type == BodyType::BIN_FILE)){
		if(!std::filesystem::exists(response.body_message)){
			std::cout << "    FILE DOES NOT EXIST: " << response.body_message << "\n";
			response.status_code = ResponseCode::NotFound;
			response.reason = "Not Found";
			response.body_type = BodyType::TEXT;
			response.body_message ="Not Found";
		}
	}
	
	if(response.body_type == BodyType::TEXT){
		response.headers["Content-Type"] = "text/plain; charset=utf-8";
		response.headers["Content-Length"] = std::to_string(response.body_message.length());
	}
	
	if(response.body_type == BodyType::TXT_FILE){
		response.headers["Content-Type"] = "text/html; charset=utf-8";
		response.headers["Content-Length"] = get_file_size(response.body_message);
	}
	
	std::string response_string = response.protocol_version+" "+std::to_string((int)response.status_code)+" "+response.reason+"\r\n";
	for (const auto& [key, value] : response.headers){
		response_string += (key+": "+value+"\r\n");
	}
	response_string += "\r\n";
	if(response.body_type == BodyType::TEXT){
		response_string += response.body_message;
	}
	if(response.body_type == BodyType::TXT_FILE){
		std::ifstream inFile;
		inFile.open(response.body_message); //open the input file
		std::stringstream strStream;
		strStream << inFile.rdbuf(); //read the file
		response_string += strStream.str(); //str holds the content of the file
	}
	return response_string;
}
