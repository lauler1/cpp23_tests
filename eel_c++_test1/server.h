#ifndef _SERVER_H_
#define _SERVER_H_

#include <cstdint>
#include <string>
#include <string_view>
#include <map>
#include <functional>
#include <optional>
#include <tuple>
#include <iostream>
#include "mime.h"

//#include "platform.h" //implementation dependent, e.g.: linux.cpp

#include <memory>

/**
	Errors, including the platform dependent ones ones
*/
enum class ServError {
	NO_ERROR = 0,     // OK, no error
	NOT_STARTED,      // Not yet started
	WRONG_INIT_PARAM, // Some initialization param is wrong
	STARTED,       	  // Already started
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
	Http Request Type
*/
enum class HttpRequestType {
	UNKNOWN = 0,   // Unknown request type
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	CONNECT,
	OPTIONS,
	TRACE,
	PATCH
};

/**
	Errors, including the platform dependent ones ones
*/
enum class BodyType {
	NONE = 0, // No body
	TEXT,     // Simple text in the HTTP response body
	TXT_FILE, // Send a text file
	BIN_FILE, // Send a binary file

};

enum class ResponseCode {
	Continue = 100,
	SwitchingProtocols = 101,
	EarlyHints = 103,
	OK = 200, //the request was successfully treated by the server
	Created = 201,
	Accepted = 202,
	NonAuthoritativeInformation = 203,
	NoContent = 204,
	ResetContent = 205,
	PartialContent = 206,
	MultipleChoices = 300,
	MovedPermanently = 301,
	Found = 302,
	NotModified = 304,
	BadRequest = 400, //the request sent by the client is erroneous (malformed request, syntax error...)
	Unauthorized = 401,
	Forbidden = 403,
	NotFound = 404, //the server did not find the resource requested by the client.
	MethodNotAllowed = 405,
	RequestTimeout = 408,
	ImATeapot = 418,
	InternalServerError = 500, //“the server encountered an unexpected condition that prevented it from fulfilling the request.”
	NotImplemented = 501,
	BadGateway = 502,
	ServiceUnvailable = 503,
	GatewayTimeout = 504,
	HttpVersionNotSupported = 505
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
	Raw data exchangeds via socket.
	For internal use only
*/
struct EventData {
	EventData() : fd{0}, length{0}, cursor{0}, capacity_{100240} {buffer = new char[capacity_+1];}
	EventData(size_t capacity) : fd{0}, length{0}, cursor{0}, capacity_{capacity} {buffer = new char[capacity_+1];}
	~EventData() {delete[] buffer;}
	int id;
	int fd;
	bool keep_alive{false};
	size_t length; // number of bytes
	size_t cursor; // for parcial reception/transmission
	char *buffer;
	
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
		std::cout << ":       length = " << length<< "\n";
		std::cout << ":       cursor = " << cursor<< "\n";

	}

	private:
		size_t capacity_; // Max allowed number of bytes
};

/**
	Http request data
*/
struct HttpRequest{
	/*
	https://www.practical-go-lessons.com/chap-26-basic-http-server
	https://www.aisangam.com/blog/http-request-message-format-well-explained/
	*/
	HttpRequest() = default;
	EventData* event_data_ptr{nullptr};	
	//HttpRequestType method_type {HttpRequestType::UNKNOWN};
	std::string method{""};
	std::string uri{"/"};
	std::string resource{""};
	std::string protocol_version{"HTTP/1.1"};
	std::map<std::string, std::string> headers{};
	char message_body[10240];
	/*
	std::optional<std::string> get_header(std::string key) const {
		
		if(headers.find(key) != headers.end()){
			return headers[key];
		}
		return {};
	}*/

};

/**
	Http response data
*/
struct HttpResponse{
	HttpResponse() = default;
	//EventData* event_data_ptr{nullptr};	
	std::string protocol_version{"HTTP/1.1"};
	ResponseCode status_code{ResponseCode::OK};
	std::string reason{"OK"};
	std::map<std::string, std::string> headers{};//{"Connection", "close"}};
	
	/**
	if body_type = BodyType::NONE, body_message is ignored
	if body_type = BodyType::TEXT, body_message is text  
	if body_type = BodyType::TXT_FILE, body_message is the file name to be load (For text file)
	if body_type = BodyType::BIN_FILE, body_message is the file name to be load (For binary file)
	*/
	BodyType body_type{BodyType::NONE};
	/**
	Contyains the body text to be transmitted (if BodyType::TEXT) or the file path name (if BodyType::TXT_FILE or BodyType::BIN_FILE)
	*/
	std::string body_message{""};
};

struct Request_Callback_Interface{
	/**
		Callback to process a request from the server and to answer with response.
		The caller chall ensure the correct allocation of the parameters.
		Pure virtual function.
		
		return: true if message was correctly consumed and response is ready.
	*/
	virtual bool on_request(HttpRequest* request, HttpResponse* response) = 0; // pure virtual
	//virtual bool on_error(const HttpRequest* request, HttpResponse* response) = 0;   // pure virtual
};

/**
	Micro Http server.
	It has a sub class containing the platform dependent implementation
*/
class HttpServer{
	ServInit conf_;
	Request_Callback_Interface &proc_request_; // Peer object to process the receiver requests
	//std::function<void(HttpRequest*, HttpResponse*)> proc_request_;
	
	public:
		HttpServer() = delete;
		HttpServer(ServInit params, Request_Callback_Interface &on_request);//std::function<void(HttpRequest*, HttpResponse*)> on_request);
		~HttpServer();
		int start(std::string_view start_page);
		int run();
		
		std::tuple<std::string, size_t, char*> proc_raw_request(EventData* event_data_ptr);
		
	private:	
		class PlatSocketImpl;// Forward declaration of the platform dependent implementation class
		std::unique_ptr<PlatSocketImpl> pImpl; // Pointer to platform dependent implementation
};

/**
	Function that tryes to open an URL in a browser.
	Platform dependent
*/
extern int open_browser(std::string url);

extern std::string get_sec_websocket_accept(std::string key);

extern void save_buffer(std::string filename, const char* buffer, size_t len);
#endif // _SERVER_H_
