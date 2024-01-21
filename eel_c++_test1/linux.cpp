#include "server.h"
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

static int epoll_id = 1001; // For debug only


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
		int sock_fd_{0};
		int epollfd_{0};
		std::string host_;
		std::uint16_t port_;
		static constexpr int BACKLOG_SIZE = 10;
		static constexpr int MAX_EVENTS = 50;
		struct epoll_event ev_{};
		struct epoll_event events_[MAX_EVENTS];
		
		ServError serv_listen();

};

HttpServer::HttpServer(ServInit params, Request_Callback_Interface &proc_request): conf_{params},
				proc_request_{proc_request},
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
	
	epollfd_ = epoll_create1(0);
	if (epollfd_ == -1) {
		return ServError::EPOLL_CREATE;
	}

	ev_.events = EPOLLIN;
	EventData* event_data_ptr = new(EventData);
	ev_.data.ptr = event_data_ptr;
	event_data_ptr->fd = sock_fd_;
	event_data_ptr->id = epoll_id++;
	if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, sock_fd_, &ev_) == -1) {
		return ServError::EPOLL_CTL;
	}
	
	started_ = true;
	return ServError::NO_ERROR;
}

ServError HttpServer::PlatSocketImpl::run_socket(){
	EventData* event_data_ptr;
	
	if(!started_){
		return ServError::NOT_STARTED;
	}
	
	int accept_fd;
	sockaddr_in client_address;
	socklen_t client_len = sizeof(client_address);
	
	std::cout << "HttpServer::PlatSocketImpl::run_socket waiting\n";
	int nfds = epoll_wait(epollfd_, events_, MAX_EVENTS, -1);
	if (nfds == -1) {
		return ServError::EPOLL_WAIT;
	}

	for (int n = 0; n < nfds; ++n) {
		event_data_ptr = reinterpret_cast<EventData*>(events_[n].data.ptr);
		int fd = event_data_ptr->fd;
		if (fd == sock_fd_) {
			std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  new connection: "<< event_data_ptr->id <<", event: "<< events_[n].events << "\n";
			accept_fd = accept4(sock_fd_, (sockaddr *)&client_address, &client_len, SOCK_NONBLOCK);

			if (accept_fd == -1) {
				return ServError::SOCK_ACCEPT;
			}

			ev_.events = EPOLLIN | EPOLLET;
			event_data_ptr = new(EventData);
			ev_.data.ptr = event_data_ptr;
			event_data_ptr->id = epoll_id++;
			event_data_ptr->fd = accept_fd;
			
			if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, accept_fd, &ev_) == -1) {
				return ServError::EPOLL_CTL;
			}
			std::cout << "						new id: "<< event_data_ptr->id << "\n";

		} else {
			std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd do_use_fd: "<< event_data_ptr->id << ", event: "<< events_[n].events << "\n";
			if ((events_[n].events & EPOLLHUP) != 0)
			{
				std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"] fd EPOLLHUP closed\n";
				delete(reinterpret_cast<EventData*>(events_[n].data.ptr));
				if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
					perror("epoll_ctl");
				}
				close(fd);
			}
			if ((events_[n].events & EPOLLIN) != 0)  {
				std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd EPOLLIN  sizeof(event_data_ptr->buffer): "<< sizeof(event_data_ptr->buffer) << "\n";
				event_data_ptr->length = recv(fd, event_data_ptr->buffer, event_data_ptr->capacity_, 0);
				event_data_ptr->buffer[event_data_ptr->length] = 0;

				if(event_data_ptr->length == 0) {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length == 0\n";

				}
				else if(event_data_ptr->length > 0) {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length: "<< event_data_ptr->length << " (do something)\n";
					event_data_ptr->fd = fd;
					
					//Do something in the main class
					auto response_string = http_server_->proc_raw_request(event_data_ptr);
					
					std::cout << "---------------------------------------------------\n";
					std::cout << " Response\n";
					std::cout << "---------------------------------------------------\n";
					std::cout << (*response_string).c_str() << "\n\n";

					if((*response_string).length() > event_data_ptr->capacity_){
						//TODO
						//delete(reinterpret_cast<EventData*>(events_[n].data.ptr)); //Delete old data.ptr
						//EventData* response_data_ptr = new EventData;              //Create a new one bigger
						//ev_.data.ptr = response_data_ptr;                          //Use the new one (bigger)
					}
					
					event_data_ptr->length = (*response_string).length();
					event_data_ptr->cursor = 0;
					std::strcpy(event_data_ptr->buffer, (*response_string).c_str());
					ev_.data.ptr = event_data_ptr;

					ev_.events = EPOLLOUT | EPOLLET;
					if (epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev_) < 0) {
						perror("epoll_ctl");
					}
			
					continue; //Process the next item in the loop
				}
				else {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length < 0: "<< event_data_ptr->length << " (error)\n";
				}
				
				std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"] fd closing\n";
				delete(reinterpret_cast<EventData*>(events_[n].data.ptr));
				if (epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr) < 0) {
					perror("epoll_ctl");
				}
				close(fd);

			}
			if ((events_[n].events & EPOLLOUT) != 0)
			{
				std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd EPOLLOUT\n";
				size_t byte_count = send(fd, event_data_ptr->buffer + event_data_ptr->cursor, event_data_ptr->length, 0);
				if (byte_count >= 0) {
					if (byte_count < event_data_ptr->length) {  // there are still bytes to write
						std::cout << "     More data to send\n";
						event_data_ptr->cursor += byte_count;
						event_data_ptr->length -= byte_count;
						ev_.data.ptr = event_data_ptr;
						ev_.events = EPOLLOUT;
						epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev_);
					} else {  // we have written the complete message -> close connection
						std::cout << "     Written the complete message -> close connection\n";
						epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr);
						close(fd);
						delete event_data_ptr;
					
					/*
						request = new EventData();
						request->fd = fd;
						ev_.data.ptr = event_data_ptr;
						ev_.events = EPOLLIN | EPOLLET;
						epoll_ctl(epollfd_, EPOLL_CTL_MOD, fd, &ev_);
						delete event_data_ptr;
						*/
					}	
				}else{
					if (errno == EAGAIN || errno == EWOULDBLOCK) {  // retry
						std::cout << "     Retry\n";
						ev_.data.ptr = event_data_ptr;
						ev_.events = EPOLLOUT;
						epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev_);
					} else {  // other error
						std::cout << "     other error\n";
						epoll_ctl(epollfd_, EPOLL_CTL_DEL, fd, nullptr);
						close(fd);
						delete event_data_ptr;
					}
				}
				

			}
		}
	}

	return ServError::NO_ERROR;
};

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
