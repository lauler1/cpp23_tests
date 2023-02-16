// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0
#ifndef _STACKTRACE_H_
#define _STACKTRACE_H_

#include <execinfo.h>
#include <cxxabi.h>

#include <memory>
#include <iostream>
#include <cstring>

namespace DBG {
	template<typename T>
	static inline std::string print_demangle(T &&t){
		// allocate string which will be filled with the demangled function name
		size_t funcnamesize = 256;
		std::unique_ptr<char []> funcname = std::unique_ptr<char[]>( (char*)malloc(funcnamesize));
		int status;
		char* ret = abi::__cxa_demangle(typeid(t).name(), &funcname[0], &funcnamesize, &status);
		if (status == 0) {

			return std::string(ret);
		}else {
			// demangling failed. Output function name as a C function with no arguments.
			return typeid(t).name();
		}
	}
	
	
	static inline void print_stacktrace(void){
		
		void *array[65];
		std::unique_ptr<char *[]> strings;
		std::unique_ptr<char []> funcname;
		int size, i;

		size = backtrace (array, 65);
		strings = std::unique_ptr<char* []>(backtrace_symbols (array, size));

		// allocate string which will be filled with the demangled function name
		size_t funcnamesize = 256;
		funcname = std::unique_ptr<char[]>( (char*)malloc(funcnamesize));
		
		if (strings.get() != nullptr)
		{
			std::cout <<  "\n\n\nObtained " << size << " stack frames.\n";
			for (i = 0; i < size; i++){
				char *bpch = std::strstr(strings[i], "_Z");
				char *epch = std::strstr(strings[i], "+");
				if ((bpch) && (epch))
				{
					*epch = '\0';
					int status;
					char* ret = abi::__cxa_demangle(bpch, &funcname[0], &funcnamesize, &status);
					if (status == 0) {
						bpch--; *bpch = '\0';
						epch = std::strstr(++epch, "[");
						std::cout << strings[i] << " \t" << ret << " " << epch << std::endl;
					}else {
						// demangling failed. Output function name as a C function with no arguments.
						std::cout <<  strings[i] << ": demangling failed" << std::endl;
					}
				}else{
					std::cout <<  strings[i] << std::endl;
				}
			}
		}
	}
}
#endif // _STACKTRACE_H_
