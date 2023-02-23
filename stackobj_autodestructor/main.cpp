// main.cpp
#include <iostream>

class widget
{
private:

public:
    widget() { std::cout << "widget constructor\n"; }
    ~widget() { std::cout << "widget destructor\n"; }
    void do_something() {std::cout << "widget do something\n";}
};

void hello(){
	widget w;
	w.do_something();
}

int main() {
	std::cout << "Begin\n";
	hello();
	std::cout << "End\n";
	return 0;
}

/*
Begin
widget constructor
widget do something
widget destructor
End

*/