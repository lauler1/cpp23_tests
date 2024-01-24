#include "server.h"
#include "logger.h"
//#include "platform.h" //e.g.: linux.cpp
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <cstring>

/*
This is a Linux only code
*/
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


//Thios is the platform specific implementation
class HttpServer::PlatSocketImpl{

	public:
		PlatSocketImpl(HttpServer *http_server, bool server, const std::string &host, std::uint16_t port):
			http_server_{http_server}, server_{server}, host_{host}, port_{port} {};
		~PlatSocketImpl() {
			std::cout << "HttpServer::PlatSocketImpl::~PlatSocketImpl\n";
			if(sock_fd_){ close(sock_fd_);}
		};
		ServError start_socket();
		ServError run_socket();
		

	private:
		HttpServer *http_server_;
		bool started_{false};
		bool server_;//[[maybe_unused]] client not implemented yet
		std::string host_;
		std::uint16_t port_;
		static constexpr int BACKLOG_SIZE = 10;
		static constexpr int MAX_EVENTS = 50;
		Logger sendlog{"sendlog.txt"};
		
		//Platform specific:
		int epoll_fd_{0};
		int sock_fd_{0};
		
		ServError serv_listen();
		static int epoll_id; // For debug only
		
		ServError set_add_epoll_ctl(int fd, uint32_t events, EventData* event_data_ptr);
		ServError set_mod_epoll_ctl(int fd, uint32_t events, EventData* event_data_ptr);
		ServError set_del_epoll_ctl(int fd, EventData* event_data_ptr);


};
int HttpServer::PlatSocketImpl::epoll_id = 0;

HttpServer::HttpServer(ServInit params, Request_Callback_Interface &on_request): conf_{params},
				proc_request_{on_request},
				pImpl{std::make_unique<PlatSocketImpl>(this, true, "0.0.0.0", params.port)} {

    std::cout << "HttpServer::HttpServer\n";
    std::cout << " conf_.port: " << conf_.port << "\n";
    std::cout << " conf_.dir: "  << conf_.dir << "\n";
    std::cout << " conf_.open_browser: "  << conf_.open_browser << "\n";
    std::cout << " conf_.default_page: "  << conf_.default_page << "\n";
    std::cout << "end\n";
}

HttpServer::~HttpServer() = default;

int HttpServer::start(std::string_view start_page){
	
	if(start_page == ""){
		start_page = conf_.default_page;
	}
	
    std::cout << "HttpServer::start\n";
	std::cout << " start_page: " << start_page << "\n";
    std::cout << "end\n";
	
	pImpl->start_socket();
	
	if(conf_.open_browser){
		open_browser(conf_.dir.append("/").append(start_page));
	}
	
	return 0;//ENOTCONN;
}

int HttpServer::run(){
    //std::cout << "HttpServer::run\n";

	ServError err = pImpl->run_socket();
	if(err != ServError::NO_ERROR){
		std::cout << "HttpServer::run err: "  << int(err) << "\n";
	}

    //std::cout << "end\n";	
	return 0;
}

ServError HttpServer::PlatSocketImpl::start_socket(){
	std::cout << "HttpServer::PlatSocketImpl::start_socket\n";
	if(started_){
		return ServError::STARTED;
	}
	
	int opt = 1;
	sockaddr_in server_address;
	
	if ((sock_fd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)) < 0) {
		return ServError::SOCKET;
	}
	
	if (setsockopt(sock_fd_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
		sizeof(opt)) < 0) {
		return ServError::SOCK_OPTS;
	}
	
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	inet_pton(AF_INET, host_.c_str(), &(server_address.sin_addr.s_addr));
	server_address.sin_port = htons(port_);

	if (bind(sock_fd_, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
		return ServError::SOCK_BIND;
	}
		
	if(server_){
		if(http_server_ == nullptr){
			return ServError::WRONG_INIT_PARAM;
		}
		return serv_listen();
	}
	
	/*TODO client version  ... */
	started_ = true;
	return ServError::NO_ERROR;
}

ServError HttpServer::PlatSocketImpl::serv_listen(){

	if (listen(sock_fd_, BACKLOG_SIZE) < 0) {
		return ServError::SOCK_LISTEN;
	}
	std::cout << "HttpServer::PlatSocketImpl::serv_listen listening...\n";
	
	epoll_fd_ = epoll_create1(0);
	if (epoll_fd_ == -1) {
		return ServError::EPOLL_CREATE;
	}

	EventData* event_data_ptr = new(EventData);
	event_data_ptr->fd = sock_fd_;
	event_data_ptr->id = epoll_id++;

	ServError err=set_add_epoll_ctl(sock_fd_, EPOLLIN, event_data_ptr);
	if(err != ServError::NO_ERROR) return err;
	
	started_ = true;
	return ServError::NO_ERROR;
}

ServError HttpServer::PlatSocketImpl::run_socket(){

	struct epoll_event events_[MAX_EVENTS];
	if(!started_){
		return ServError::NOT_STARTED;
	}
	
	sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	
	std::cout << "HttpServer::PlatSocketImpl::run_socket waiting\n";
	int nfds = epoll_wait(epoll_fd_, events_, MAX_EVENTS, -1);
	if (nfds == -1) {
		return ServError::EPOLL_WAIT;
	}

	EventData* event_data_ptr;
	for (int n = 0; n < nfds; ++n) {
		event_data_ptr = reinterpret_cast<EventData*>(events_[n].data.ptr);
		
		event_data_ptr->print("run_socket For Loop");
		
		int fd = event_data_ptr->fd;
		if (fd == sock_fd_) {
			std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  new connection: "<< event_data_ptr->id <<", event: "<< events_[n].events << "\n";
			int accept_fd = accept4(sock_fd_, (sockaddr *)&client_address, &client_len, SOCK_NONBLOCK);

			if (accept_fd == -1) {
				return ServError::SOCK_ACCEPT;
			}

			event_data_ptr = new(EventData);
			event_data_ptr->id = epoll_id++;
			event_data_ptr->fd = accept_fd;

std::cout << "2\n";
			ServError err=set_add_epoll_ctl(accept_fd, EPOLLIN | EPOLLET, event_data_ptr);
			if(err != ServError::NO_ERROR) return err;
		} else {
			std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd do_use_fd: "<< event_data_ptr->id << ", event: "<< events_[n].events << "\n";
			if ((events_[n].events & EPOLLIN) != 0)  {
				std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd EPOLLIN  sizeof(event_data_ptr->buffer): "<< sizeof(event_data_ptr->buffer) << "\n";
				event_data_ptr->print("EPOLLIN");
				event_data_ptr->length = recv(fd, event_data_ptr->buffer, event_data_ptr->get_capacity(), 0);
				event_data_ptr->buffer[event_data_ptr->length] = 0;

				if(event_data_ptr->length == 0) {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length == 0\n";
					//continue;

				}
				else if(event_data_ptr->length > 0) {
					event_data_ptr->print("EPOLLIN if event_data_ptr->length > 0");
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length: "<< event_data_ptr->length << " (do something)\n";
					event_data_ptr->fd = fd;
					
					//Do something in the main class
					auto [response_string, buff_size, bin_buff] = http_server_->proc_raw_request(event_data_ptr);
					
					if(response_string == ""){//This is a websocket
						std::cout << "---------------------------------------------------\n";
						std::cout << " Websocket Response\n";
						std::cout << "---------------------------------------------------\n";
						std::cout << "    To send " << buff_size << " bytes\n";
						if(bin_buff != nullptr){
							bin_buff[buff_size] = 0;
							std::cout << "    Message:\n" << bin_buff << "\n\n";
						}else{
							std::cout << "    Will be closed\n\n";
							event_data_ptr->length = 0;
						}
						
						// Websockets uses bin_buff = event_data_ptr->buffer, no delete needed
						//continue;

					}else{
					
						std::cout << "---------------------------------------------------\n";
						std::cout << " Http Response\n";
						std::cout << "---------------------------------------------------\n";
						std::cout << response_string.c_str() << "\n\n";

						auto expected_capacity = response_string.length() + buff_size;
						
						// Need more space?
						if(event_data_ptr->get_capacity() < expected_capacity){
							auto old_id = event_data_ptr->id;
							auto old_keep_alive = event_data_ptr->keep_alive;
							delete(event_data_ptr); //Delete old data.ptr
							
							event_data_ptr = new EventData(expected_capacity); //Create a new one bigger
							event_data_ptr->fd = fd;
							event_data_ptr->id = old_id;
							event_data_ptr->keep_alive = old_keep_alive;
						}
						
						// Binary data?
						if(bin_buff != nullptr and buff_size > 0){

							std::memcpy(event_data_ptr->buffer, response_string.c_str(), response_string.length());
							std::memcpy(&event_data_ptr->buffer[response_string.length()], bin_buff, buff_size);

							event_data_ptr->length = expected_capacity;
							delete[] bin_buff;

						}else{
							std::strcpy(event_data_ptr->buffer, response_string.c_str());
							event_data_ptr->length = response_string.length();
						}
					}

					if(event_data_ptr->length > 0){ // If data to send
						event_data_ptr->cursor = 0;
						ServError err=set_mod_epoll_ctl(fd, EPOLLOUT | EPOLLET, event_data_ptr);
						if(err != ServError::NO_ERROR) return err;
					}else if(!event_data_ptr->keep_alive){

						ServError err=set_del_epoll_ctl(fd, event_data_ptr);
						if(err != ServError::NO_ERROR) return err;
						continue; //After deleting a event_data_ptr, no further event handling is possible.
					}

					continue; //Process the next item in the loop
				}
				else {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length < 0: "<< event_data_ptr->length << " (error)\n";
				}
				
				ServError err=set_del_epoll_ctl(fd, event_data_ptr);
				if(err != ServError::NO_ERROR) return err;
				continue; //After deleting a event_data_ptr, no further event handling is possible.
			}
			if ((events_[n].events & EPOLLOUT) != 0)
			{
				std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd EPOLLOUT "<<event_data_ptr->length<<"\n";
				event_data_ptr->print("EPOLLOUT");
				ssize_t byte_count = send(fd, event_data_ptr->buffer + event_data_ptr->cursor, event_data_ptr->length, 0);
				sendlog.log(std::string_view(event_data_ptr->buffer + event_data_ptr->cursor, event_data_ptr->length));
				//save_buffer("log3.txt", event_data_ptr->buffer + event_data_ptr->cursor, event_data_ptr->length, true);
				if (byte_count >= 0) {
					if (byte_count < event_data_ptr->length) {  // there are still bytes to write
						std::cout << "     More data to send\n";
						event_data_ptr->cursor += byte_count;
						event_data_ptr->length -= byte_count;

						ServError err=set_mod_epoll_ctl(fd, EPOLLOUT | EPOLLET, event_data_ptr);
						if(err != ServError::NO_ERROR) return err;

					} else {  // we have written the complete message -> close connection?
						std::cout << "     Written the complete message -> close connection? sent="<<byte_count<<"\n";
						
						if(event_data_ptr->keep_alive){
							std::cout << "         Keep alive\n";
							event_data_ptr->reset();
							ServError err=set_mod_epoll_ctl(fd, EPOLLIN | EPOLLET, event_data_ptr);
							if(err != ServError::NO_ERROR) return err;

						}else{
							ServError err=set_del_epoll_ctl(fd, event_data_ptr);
							if(err != ServError::NO_ERROR) return err;
							continue; //After deleting a event_data_ptr, no further event handling is possible.
						}
					}	
				}else{
					std::cout << "some error\n";
					ServError err=set_del_epoll_ctl(fd, event_data_ptr);
					if(err != ServError::NO_ERROR) return err;
					continue; //After deleting a event_data_ptr, no further event handling is possible.
				}
			}
			if ((events_[n].events & EPOLLHUP) != 0)
			{
				ServError err=set_del_epoll_ctl(fd, event_data_ptr);
				if(err != ServError::NO_ERROR) return err;
			}
		}
	}

	return ServError::NO_ERROR;
};


ServError HttpServer::PlatSocketImpl::set_add_epoll_ctl(int fd, uint32_t events, EventData* event_data_ptr){
	struct epoll_event ev_{};
	if(event_data_ptr == nullptr){
		return ServError::WRONG_INIT_PARAM;
	}
	std::cout << "EPOLL: Add: fd="<<fd<<" as "<<(events&EPOLLOUT?"OUT":"IN")<<"\n";
	std::cout << "            id="<<event_data_ptr->id<<"\n";
	event_data_ptr->fd = fd;
	ev_.data.ptr = event_data_ptr;
	ev_.events = events;
std::cout << " 1\n";
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev_) == -1) {
std::cout << " 2\n";
		return ServError::EPOLL_CTL;
	}
std::cout << " 3\n";
	return ServError::NO_ERROR;
}

ServError HttpServer::PlatSocketImpl::set_mod_epoll_ctl(int fd, uint32_t events, EventData* event_data_ptr){
	struct epoll_event ev_{};
	if(event_data_ptr == nullptr){
		return ServError::WRONG_INIT_PARAM;
	}
	std::cout << "EPOLL: Change: fd="<<fd<<" to "<<(events&EPOLLOUT?"OUT":"IN")<<"\n";

	event_data_ptr->fd = fd;
	ev_.data.ptr = event_data_ptr;
	ev_.events = events;
	if (epoll_ctl(epoll_fd_, EPOLL_CTL_MOD, fd, &ev_) < 0) {
		perror("epoll_ctl");
		return ServError::EPOLL_CTL;
	}	
	return ServError::NO_ERROR;
}

ServError HttpServer::PlatSocketImpl::set_del_epoll_ctl(int fd, EventData* event_data_ptr){
	if(event_data_ptr == nullptr){
		return ServError::WRONG_INIT_PARAM;
	}
	std::cout << "EPOLL: Close: fd="<<fd<<"\n";

	if (epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
		perror("epoll_ctl");
		return ServError::EPOLL_CTL;
	}
	close(fd);
	delete event_data_ptr;
	return ServError::NO_ERROR;
}


/**
	Verifies if this app is running in a WSL environment using `std::system`
*/
bool is_wsl(){
	std::string file_content;
	std::system("uname -a >log.txt"); // executes the UNIX command "uname -a and store output in log.txt"
	std::getline(std::ifstream("log.txt"), file_content, '\0'); // Read log.txt in a string
    std::cout << "is_wsl\n";
    std::cout << " log.txt: " << file_content << "\n";
    std::cout << "end\n";
	if(file_content.find("WSL")){
		return true;
	}
	return false;
}


/**
	Open a url in the default browser using `std::system`.
	If running in WSL, it uses the `wslview` app that opens the browser in the Windows instead of Linux.
	Otherwise it use the default Linux browser
	
	Note: To use  `wslview` , it must be furs installed: https://github.com/wslutilities/wslu
*/
int open_browser(std::string url) {
	
	std::string s{"open "};
	if(is_wsl()){
		s = "wslview ";
	}
	s.append(url);
    std::cout << "open_browser\n";
    std::cout << " s: " << s << "\n";
    std::cout << "end\n";
	std::system(s.c_str());
    
    return 0;
}
