#include "logger.h"

/*
#include <iostream>
#include <fstream>
#include <cerrno>
#include <sstream>
#include <regex>
#include <filesystem>
#include <cassert>
*/

#include <format>
#include <iostream>
#include <chrono>
#include <ctime>

Logger::Logger(const std::string_view filename, bool append){
	std::ios_base::openmode mode = std::ios::binary | std::ios::out;
	if(append){
		mode |= std::ios::app;
	}
	outfile.open(filename.data(), mode);
	
	if (outfile.is_open()) { // check if the file was opened successfully
	std::cout << filename << " LOG INIT" << std::endl;
		log("LOG INIT");

		auto now = std::chrono::system_clock::now();// Get the current time
		std::time_t now_c = std::chrono::system_clock::to_time_t(now); // Convert to a time_point
		log(std::ctime(&now_c));// Format as a string
	}
	else {
		std::cerr << filename << " Error opening file" << std::endl;
	}

}

Logger::~Logger(){
	outfile.close();
}

void Logger::log(const std::string_view msg, const std::source_location& location){
	std::string log_str = std::format("{}:{}: {}\n", location.file_name(), location.line(), msg);
	std::cout << " LOG INIT: " << log_str << std::endl;
	outfile.write(log_str.data(), log_str.size());
	outfile.flush();
}

//std::cout << std::format("{} {}!", "Hello", "world", "something") << "\n";