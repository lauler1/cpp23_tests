// main.c
#include <stdio.h>
/*
#include <iostream>
//#include <stacktrace>
#include <string>
#include <array>
#include <assert.h>*/


int main () {

	static const char data[] = {
#embed <data.txt>
	};
	
	printf ("Begin\n");
/*
	std::cout << "\nStrings\n\n";
	
	std::cout << data << std::endl;
	std::string s0 ("Initial string");

	// constructors used in the same order as described above:

	std::basic_string_view<char> s3 = reinterpret_cast<char*>(data);
	std::cout << "s3: " << s3 << "\n";
	
	std::basic_string<unsigned char> s3b = data;
	std::cout << "a1 len: " << s3b.c_str() << "\n";

	std::string mst1 = std::string(reinterpret_cast<char*>(data), readme_md_len);
	std::cout << "mst1: " << mst1 << "\n";

	auto a1 = std::to_array<unsigned char>(data);
	std::cout << "a1 len: " << a1.size() << "\n";
	
	std::cout << "\nNull pointer\n\n";


	std::cout << "End\n";
*/
	printf ("End\n");
	return 0;
}


/*
Begin
widget constructor
widget do something
widget destructor
End

*/
