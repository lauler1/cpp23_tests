#include <iostream>
#include <sstream>
#include <streambuf>
#include <cassert>

// Class to save the current streambuf of std::cin and replace it with a custom one
class CinRedirect {
    std::streambuf* oldCinStreamBuf;
    std::istringstream testInput;
public:
    CinRedirect(const std::string& input) : testInput(input) {
        oldCinStreamBuf = std::cin.rdbuf(); // save old buf
        std::cin.rdbuf(testInput.rdbuf()); // redirect std::cin
    }

    ~CinRedirect() {
        std::cin.rdbuf(oldCinStreamBuf); // restore old buf
    }
};

// Function to test
int addTwoNumbers() {
    int x, y;
	std::cout << "Input 1:\n";
    std::cin >> x;
	std::cout << "Input 2:\n";
    std::cin >> y;
    return x + y;
}

// A test function
void test_addTwoNumbers() {
    CinRedirect cr("3\n3\n");
    assert(addTwoNumbers() == 6);
}

int main() {
    // Run the tests
    test_addTwoNumbers();

    std::cout << "All tests passed.\n";
    return 0;
}

/*
Alternative 2, instead of redirect std::cin, we use std::istream& input as a replacement
```c++
// Function to test
int addTwoNumbers(std::istream& input) {
    int x, y;
    input >> x;
    input >> y;
    return x + y;
}

void test_addTwoNumbers() {
    std::istringstream testInput("2\n3\n");
    assert(addTwoNumbers(testInput) == 5);
}
```

*/