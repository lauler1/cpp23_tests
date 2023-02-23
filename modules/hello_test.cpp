module;
#include <iostream>
export module hello_test; // module declaration
export void hello() {
  std::cout << "Hello World!\n";
}
