#include "tcpserver.h"
#include "sha1.h"

#include <iostream>
#include <fstream>
#include <cerrno>
#include <sstream>
#include <regex>
#include <filesystem>
#include <cassert>

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

bool TcpServer::proc_raw_request(EventData* event_data_ptr){
	assert(event_data_ptr != nullptr);
	
	std::cout << "TcpServer::proc_raw_request\n";
	
	return decorator_.proc_raw_request(event_data_ptr);
	

}

