#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string_view>
//#include <iostream>
#include <source_location>
#include <fstream>
/*
#include <sstream>
#include <iomanip>

#include <cstdint>
#include <string>
#include <cstring>
#include <map>
#include <functional>
#include <optional>
#include <tuple>
#include <iostream>
#include <format>
#include "mime.h"
#include <memory>
*/

/**
	Micro Logger
*/
class Logger{
	
	public:
		Logger() = delete;
		Logger(const std::string_view filename, bool append = false);
		~Logger();
		
		void log(const std::string_view msg, const std::source_location& location = std::source_location::current());
		
	private:	
		std::ofstream outfile;
};


#endif // _LOGGER_H_
