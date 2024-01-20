#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdint>
#include <string>
#include <string_view>
//#include "platform.h" //implementation dependent, e.g.: linux.cpp

#include <memory>

/**
	Errors, including the platform dependent ones ones
*/
enum class ServError {
	NO_ERROR = 0,  // OK, no error
	NOT_STARTED,   // Not yet started
	STARTED,       // Already started
	SOCKET,
	SOCK_OPTS,
	SOCK_BIND,
	SOCK_ACCEPT,
	SOCK_LISTEN,
	EPOLL_CREATE,
	EPOLL_CTL,
	EPOLL_WAIT
	
};

/**
	Raw data exchangeds via socket
*/
struct EventData {
	EventData() : fd(0), length(0), cursor(0), buffer() {}
	int fd;
	int id;
	size_t length; // number of bytes
	size_t cursor; // for parcial reception/transmission
	char buffer[10240];
};

/**
	HttpServer initialization attributes
*/
struct ServInit {
	uint16_t port{8000};
	std::string dir{"web"};
	bool open_browser{true};
	std::string default_page{"index.html"};
};

/**
	Micro Http server.
	It has a sub class containing the platform dependent implementation
*/
class HttpServer{
	ServInit conf;
	//PlatSocket server;
	
	public:
		HttpServer(ServInit params);
		~HttpServer();
		int start(std::string_view start_page);
		int run();
		
		
		
		class PlatSocketImpl;// Forward declaration of the platform dependent implementation class
		std::unique_ptr<PlatSocketImpl> pImpl; // Pointer to platform dependent implementation
};

/**
	Function that tryes to open an URL in a browser.
	Platform dependent
*/
extern int open_browser(std::string url);

#endif // _SERVER_H_
