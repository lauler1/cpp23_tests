#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string_view>
#include <source_location>
#include <fstream>

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
