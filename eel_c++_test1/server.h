// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdint>
#include <string>
#include <string_view>

struct ServInit {
	uint16_t port = 8000;
	std::string dir{ "web" };
};

class Server{
	ServInit conf;
	
	public:
		Server(ServInit params);
		int start(std::string_view start_page);
		int run();

};

#endif // _SERVER_H_
