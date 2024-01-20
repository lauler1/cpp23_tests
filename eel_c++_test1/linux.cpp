#include "server.h"
//#include "platform.h" //e.g.: linux.cpp
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

/*
This is a Linux only code
*/
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

static int epoll_id = 1001; // For debug only

void do_something(EventData* event_data_ptr){
	event_data_ptr->buffer[event_data_ptr->length] = 0;
	std::cout << "\n do_something:\n" << event_data_ptr->buffer << " \n";
}

//Thios is the platform specific implementation
class HttpServer::PlatSocketImpl{

	public:
		PlatSocketImpl(bool server, const std::string &host, std::uint16_t port):
			server_(server), host_(host), port_(port) {
				
			};
		~PlatSocketImpl() {
			std::cout << "HttpServer::PlatSocketImpl::~PlatSocketImpl\n";
			if(sock_fd_){ close(sock_fd_);}
		};
		ServError start_socket();
		ServError run_socket();
		

	private:
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


HttpServer::HttpServer(ServInit params): conf{params},  //server(true, "0.0.0.0", params.port)
				pImpl(std::make_unique<PlatSocketImpl>(true, "0.0.0.0", params.port)) {

    std::cout << "HttpServer::HttpServer\n";
    std::cout << " conf.port: " << conf.port << "\n";
    std::cout << " conf.dir: "  << conf.dir << "\n";
    std::cout << " conf.open_browser: "  << conf.open_browser << "\n";
    std::cout << " conf.default_page: "  << conf.default_page << "\n";
    std::cout << "end\n";
}

HttpServer::~HttpServer() = default;

int HttpServer::start(std::string_view start_page){
	
	if(start_page == ""){
		start_page = conf.default_page;
	}
	
    std::cout << "HttpServer::start\n";
	std::cout << " start_page: " << start_page << "\n";
    std::cout << "end\n";
	
	pImpl->start_socket();
	
	if(conf.open_browser){
		open_browser(conf.dir.append("/").append(start_page));
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
				event_data_ptr->length = recv(fd, event_data_ptr->buffer, sizeof(event_data_ptr->buffer), 0);

				if(event_data_ptr->length == 0) {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length == 0\n";

				}
				else if(event_data_ptr->length > 0) {
					std::cout << "HttpServer::PlatSocketImpl::run_socket["<<n<<"]  fd event_data_ptr->length: "<< event_data_ptr->length << " (do something)\n";
					event_data_ptr->fd = fd;
					
					//Do something
					do_something(event_data_ptr);
					
					//continue; //Process the next item in the loop
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
