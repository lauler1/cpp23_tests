#include "server.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

/*
This is a Linux only code
*/


/**
	Verifies if this app is running in a WSL environment using `std::system`
*/
bool is_wsl(){
	std::string file_content;
	std::system("uname -a >log.txt"); // executes the UNIX command "uname -a and store output in log.txt"
	std::getline(std::ifstream("log.txt"), file_content, '\0'); // Read log.txt in a string
    std::cout << " log.txt: " << file_content << "\n";
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
	
    std::cout << "open_browser" << std::endl;
    std::cout << " s: " << s << "\n";
    std::cout << "end" << std::endl;
	std::system(s.c_str());
    
    return 0;
}
