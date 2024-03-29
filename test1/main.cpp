// main.cpp
#include <iostream>
//#include <stacktrace>
#include <string>
#include <array>

unsigned char readme_md[] = {
  0x54, 0x68, 0x69, 0x73, 0x20, 0x69, 0x73, 0x20, 0x61, 0x20, 0x76, 0x65,
  0x72, 0x79, 0x20, 0x73, 0x69, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x65, 0x78,
  0x61, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x73, 0x68, 0x6f, 0x77, 0x69, 0x6e,
  0x67, 0x20, 0x74, 0x68, 0x61, 0x74, 0x20, 0x6f, 0x6a, 0x65, 0x63, 0x74,
  0x73, 0x20, 0x63, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x69, 0x6e,
  0x20, 0x74, 0x68, 0x65, 0x20, 0x73, 0x74, 0x61, 0x63, 0x6b, 0x20, 0x28,
  0x69, 0x6e, 0x20, 0x61, 0x20, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f,
  0x6e, 0x20, 0x63, 0x6f, 0x6e, 0x74, 0x65, 0x78, 0x74, 0x29, 0x20, 0x61,
  0x72, 0x65, 0x20, 0x61, 0x75, 0x74, 0x6f, 0x6d, 0x61, 0x74, 0x69, 0x63,
  0x61, 0x6c, 0x79, 0x20, 0x64, 0x65, 0x73, 0x74, 0x72, 0x75, 0x63, 0x74,
  0x65, 0x64, 0x20, 0x77, 0x68, 0x65, 0x6e, 0x20, 0x6c, 0x65, 0x61, 0x76,
  0x69, 0x6e, 0x67, 0x20, 0x74, 0x68, 0x65, 0x20, 0x63, 0x6f, 0x6e, 0x74,
  0x65, 0x78, 0x74, 0x20, 0x28, 0x66, 0x75, 0x6e, 0x63, 0x74, 0x69, 0x6f,
  0x6e, 0x20, 0x72, 0x65, 0x74, 0x75, 0x72, 0x6e, 0x29, 0x2e, 0x0d, 0x0a,
  0x0d, 0x0a
};
unsigned int readme_md_len = 170;

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

	std::cout << "\nStrings\n\n";
	
	std::cout << readme_md << std::endl;
	std::string s0 ("Initial string");

	// constructors used in the same order as described above:

	std::basic_string_view<char> s3 = reinterpret_cast<char*>(readme_md);
	std::cout << "s3: " << s3 << "\n";
	
	std::basic_string<unsigned char> s3b = readme_md;
	std::cout << "a1 len: " << s3b.c_str() << "\n";

	std::string mst1 = std::string(reinterpret_cast<char*>(readme_md), readme_md_len);
	std::cout << "mst1: " << mst1 << "\n";

	auto a1 = std::to_array<unsigned char>(readme_md);
	std::cout << "a1 len: " << a1.size() << "\n";
	
	std::cout << "\nNull pointer\n\n";



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