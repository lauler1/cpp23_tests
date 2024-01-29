#ifndef _HTTPSERVER_H_
#define _HTTPSERVER_H_


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

enum class WebSocketOpCode : unsigned char
{
	Continuation = 0X0, // This frame continues the payload from the previous frame.
	Text= 0x1,          // Denotes a text frame. Text frames are UTF-8 decoded by the server.
	Binary = 0x2,       // Denotes a binary frame. Binary frames are delivered unchanged by the server.
	// 0x03-0x07	Reserved for future use.
	Close = 0x8,        // Denotes the client wishes to close the connection.
	Ping = 0x9,         // A ping frame. Serves as a heartbeat mechanism ensuring the connection is still alive. The receiver must respond with a pong.
	Pong = 0xA,         // A pong frame. Serves as a heartbeat mechanism ensuring the connection is still alive. The receiver must respond with a ping frame.
    // 0x0b-0x0f	Reserved for future use.
};

struct WebSocketHeaderRaw
{
	bool final_fragment{true};

	WebSocketOpCode op_code{WebSocketOpCode::Text};
	bool is_masked{false};
	std::uint64_t payload_length{0};
	unsigned int payload_shift{2};
	unsigned char *masks{nullptr};

	WebSocketHeaderRaw() {};
	
	void set_len(std::uint64_t length){
		is_masked = false;
		payload_length = length;
		if(length > 0xFFFF){
			payload_shift = 10;
		}else if(length > 125){
			payload_shift = 4;
		}else{
			payload_shift = 2;
		}
	}
	
	/* Assume that bytes has the complete header with correct length, which varies from 2 to 14 bytes. */
	void load_from_bytes(unsigned char* bytes){
		
		final_fragment = (bytes[0] >> 7);
		op_code = (WebSocketOpCode)(bytes[0] & 0x0F);
		is_masked = (bytes[1] >> 7);
		payload_length = (bytes[1] & 0x7F);
		
		if(payload_length == 126){
			payload_length = (((std::uint64_t)bytes[2]) << 8) | (std::uint64_t)bytes[3];
			payload_shift += 2;
		}else if (payload_length == 127){
			payload_length = (std::uint64_t)bytes[9];
			payload_length |= ((std::uint64_t)bytes[8]) << 8;
			payload_length |= ((std::uint64_t)bytes[7]) << 16;
			payload_length |= ((std::uint64_t)bytes[6]) << 24;
			payload_length |= ((std::uint64_t)bytes[5]) << 32;
			payload_length |= ((std::uint64_t)bytes[4]) << 40;
			payload_length |= ((std::uint64_t)bytes[3]) << 48;
			payload_length |= ((std::uint64_t)bytes[2]) << 56;
			payload_shift += 8;
		}
		if(is_masked){
			masks = &bytes[payload_shift];
			payload_shift += 4;
		}
	}
	
	/* Assume that buff has enought space for the header, which varies from 2 to 14 bytes. */
	void write_to_buff(unsigned char *buff){
		
		buff[0]  = (((unsigned char)final_fragment) << 7);
		buff[0] |= (((unsigned char)op_code) & 0x0F);
		buff[1]  = (((unsigned char)is_masked) << 7);	
		
		if(payload_length > 0xFFFF){
			buff[1] |= (127 & 0x7F);
			buff[2]  = (unsigned char)((payload_length >> 56) & 0xFF);
			buff[3]  = (unsigned char)((payload_length >> 48) & 0xFF);
			buff[4]  = (unsigned char)((payload_length >> 40) & 0xFF);
			buff[5]  = (unsigned char)((payload_length >> 32) & 0xFF);
			buff[6]  = (unsigned char)((payload_length >> 24) & 0xFF);
			buff[7]  = (unsigned char)((payload_length >> 16) & 0xFF);
			buff[8]  = (unsigned char)((payload_length >> 8) & 0xFF);
			buff[9]  = (unsigned char)(payload_length & 0xFF);

			payload_shift = 10;
		}else if (payload_length > 125){
			buff[1] |= (126 & 0x7F);
			buff[2]  = (unsigned char)(payload_length >> 8);
			buff[3]  = (unsigned char)(payload_length & 0xFF);
			
			payload_shift = 4;
		}else{
			buff[1] |= (unsigned char)(payload_length & 0x7F);
			payload_shift = 2;
		}

		if(is_masked & (masks != nullptr)){
			
			buff[payload_shift++]  = (unsigned char)((payload_length >> 24) & 0xFF);
			buff[payload_shift++]  = (unsigned char)((payload_length >> 16) & 0xFF);
			buff[payload_shift++]  = (unsigned char)((payload_length >> 8) & 0xFF);
			buff[payload_shift++]  = (unsigned char)(payload_length & 0xFF);
			
		}

	}
	
	/*in_buffer and out_buffer contain the complete message, including header */
	void unmask(char *out_buffer, const char *in_buffer, std::uint64_t length)
	{
		if(out_buffer == nullptr or in_buffer == nullptr) {return;} //error
		
		if(is_masked){
			const char *in_ptr = &in_buffer[payload_shift];
			char *out_ptr = &out_buffer[payload_shift];
			if(length < (std::uint64_t)payload_shift)  {return;} //error
			length -= payload_shift;

			for (std::uint64_t i = 0; i < length; i++){
				out_ptr[i] = in_ptr[i] ^ masks[i % 4];
			}
		}
	}
	
	std::string to_string(){
		auto to_hex = [](int value) -> std::string{
			std::stringstream ss;
			ss << std::hex << value;
			return ss.str();
		};
		
		std::string result = "WebSocketHeaderRaw: ";
		result += "FinalFragment: "+ std::to_string(final_fragment) + ", ";
		result += "OpCode: "       + opcode_to_string(op_code)      + ", ";
		result += "IsMasked: "     + std::to_string(is_masked)      + ", ";
		result += "PayloadLength: "+ std::to_string(payload_length) + ", ";
		if(masks != nullptr){
			result += "Masks: "+ to_hex(masks[0]) + " "+ to_hex(masks[1]) + " "+ to_hex(masks[2]) + " "+ to_hex(masks[3]) + ", ";
		}
		return result;
	}
	std::string opcode_to_string(WebSocketOpCode op){
		std::string result = "";

		switch(op){
			case WebSocketOpCode::Continuation: 
				result = "Continuation";
				break;
			case WebSocketOpCode::Text: 
				result = "Text";
				break;
			case WebSocketOpCode::Binary: 
				result = "Binary";
				break;
			case WebSocketOpCode::Close: 
				result = "Close";
				break;
			case WebSocketOpCode::Ping: 
				result = "Ping";
				break;
			case WebSocketOpCode::Pong: 
				result = "Pong";
				break;
			default:
				result = "UNKNOWN";
		}

		return result;
	}
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
	unsigned long int id;

	std::string method{""};
	std::string uri{"/"};
	std::string resource{""};
	std::string protocol_version{"HTTP/1.1"};
	std::map<std::string, std::string> headers{};
	char message_body[10240];

};

/**
	Http response data
*/
struct HttpResponse{
	HttpResponse() = default;
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

/**
	Websocket income message
*/
struct WsIncome{

	WsIncome(char *const buff, const size_t buff_size): buff_ptr(buff), capacity(buff_size){};
	EventData* event_data_ptr{nullptr};
	WebSocketHeaderRaw ws_header{};
	unsigned long int id{0};
	
	size_t length{0}; // number of bytes
	char *const buff_ptr;
	const size_t capacity; // Max allowed number of bytes

};

/**
	Websocket response message
*/
struct WsResponse{
	/*
	https://www.practical-go-lessons.com/chap-26-basic-http-server
	https://www.aisangam.com/blog/http-request-message-format-well-explained/
	*/
	WsResponse(char *const buff, const size_t buff_size): buff_ptr(buff), capacity(buff_size){};
	EventData* event_data_ptr{nullptr};
	WebSocketHeaderRaw ws_header{};
	bool close_connection{false}; // Set to tru to close the WS connection

	size_t length{0}; // number of bytes
	char *const buff_ptr;
	const size_t capacity; // Max allowed number of bytes
};


struct Request_Callback_Interface{
	/**
		Callback to process a HTTP request from the server and to answer with response.
		The caller shall ensure the correct allocation of the parameters.
		Pure virtual function.
		
		return: true if message was correctly consumed and response is ready.
	*/
	virtual bool on_request(HttpRequest* request, HttpResponse* response) = 0; // pure virtual
	/**
		Callback to process a Websocket income message.
		The caller shall ensure the correct allocation of the parameters.
		Pure virtual function.
		
		Note: Websockets must first be accepted as a regular HTTP request.
		
		return: true if message was correctly consumed and response is ready.
	*/
	virtual bool on_websocket(WsIncome* income, WsResponse* response) = 0; // pure virtual
	//virtual bool on_error(const HttpRequest* request, HttpResponse* response) = 0;   // pure virtual
};

/**
	Micro Http server.
	
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
		
		void internal_unittests();
		bool proc_raw_request(EventData* event_data_ptr);
		
	private:	
		
		bool prepare_ws_response(EventData* event_data_ptr);
		bool prepare_http_response(std::string_view response_string, size_t buff_size, char* bin_buff, EventData *event_data_ptr);
		bool parse_http_request(HttpRequest &request, EventData *event_data_ptr);

};

/**
	Function that tryes to open an URL in a browser.
	Platform dependent
*/
extern int open_browser(std::string url);

extern std::string get_sec_websocket_accept_attr(std::string key);

extern void save_buffer(std::string filename, const char* buffer, size_t len, bool append = false);
#endif // _HTTPSERVER_H_
