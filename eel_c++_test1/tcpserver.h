#ifndef _SERVER_H_
#define _SERVER_H_


#include <sstream>
#include <iomanip>

#include <cstdint>
#include <string>
#include <cstring>
#include <string_view>
#include <map>
#include <functional>
#include <optional>
#include <tuple>
#include <iostream>

#include "mime.h"
#include <memory>

/**
	Errors, including the platform dependent ones ones
*/
enum class ServError {
	NO_ERROR = 0,     // OK, no error
	NOT_STARTED,      // Not yet started
	WRONG_INIT_PARAM, // Some initialization param is wrong
	STARTED,       	  // Already started
	NULL_BUFF,        // Missing buffer allocation (nullptr)
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
	Raw data exchangeds via socket.
	For internal use only
	
*/
struct EventData {
	EventData() : fd{0}, length{0}, cursor{0}, capacity_{100240} {buffer = new char[capacity_+1];buffer[capacity_] = 0;}
	EventData(size_t capacity) : fd{0}, length{0}, cursor{0}, capacity_{capacity} {buffer = new char[capacity_+1];buffer[capacity_] = 0;}
	EventData(char *buff, size_t capacity) : fd{0}, length{0}, cursor{0}, buffer{buff}, capacity_{capacity} {}
	~EventData() { if(buffer != nullptr) delete[] buffer;}
	unsigned long int id;
	int fd;
	bool websocket{false}; // Do not treat this as a HTTP socket
	bool keep_alive{false};
	ssize_t length; // number of bytes
	size_t cursor;  // for parcial reception/transmission
	char *buffer;
	
	void change_capacity(size_t capacity){
		capacity_ = capacity;
		delete buffer;
		buffer = new char[capacity_+1];buffer[capacity_] = 0;
	}
	
	void reset(){
		length = 0;
		cursor = 0;
	}
	size_t get_capacity(){return capacity_;}
	void print(std::string label = ""){
		std::cout << ":   EventData: <"<<label<<">\n";
		std::cout << ":           id = " << id<< "\n";
		std::cout << ":           fd = " << fd<< "\n";
		std::cout << ":   keep_alive = " << keep_alive<< "\n";
	}

	private:
		size_t capacity_; // Max allowed number of bytes
};

struct ServerDecoratorInterface{
	/**
		Callback decorator
		
		return: true if message was correctly consumed and response is ready.
	*/
	virtual bool proc_raw_request(EventData* event_data_ptr) = 0; // pure virtual

};

/**
	Micro TCP server.
	It has a sub class containing the platform dependent implementation.
	
	Note PlatSocketImpl: This is a sub class which is defined per platform (e.g. linux, windows) in a separate file.
	 Currently only Linux. Include in the compilation only the applicable platform file.
	
*/
class TcpServer{
	uint16_t port_;
	ServerDecoratorInterface &decorator_; // Peer object to process the receiver requests
	//std::function<void(HttpRequest*, HttpResponse*)> decorator_;
	
	public:
		TcpServer() = delete;
		TcpServer(uint16_t port, ServerDecoratorInterface &decorator);//std::function<void(HttpRequest*, HttpResponse*)> on_request);
		~TcpServer();
		ServError start();
		ServError run();
		
		bool proc_raw_request(EventData* event_data_ptr);
		
	private:	
		class PlatSocketImpl;// Forward declaration of the platform dependent implementation class
		std::unique_ptr<PlatSocketImpl> pImpl; // Pointer to platform dependent implementation


};

/**
	Function that tryes to open an URL in a browser.
	Platform dependent
*/
extern int open_browser(std::string url);

extern void save_buffer(std::string filename, const char* buffer, size_t len, bool append = false);
#endif // _SERVER_H_
