#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>

	//#include	<sys/types.h>
	//#include	<sys/gmon.h>
	//extern void _start (void), etext (void);


void prc3(){
	std::cout << "      prc3: ";
	for(int i = 0; i < 10000; i++){
		std::cout << ".";
	}
	std::cout << "\n";

}
void prc2(){
	std::cout << "    prc2\n";
	prc3();
	
}
void prc1(){
	std::cout << "  prc1\n";
	prc2();
}


int fun3(int a, int b){
	std::cout << "      fun3: ";
	for(int i = 0; i < 10000; i++){
		std::cout << "#";
	}
	std::cout << "\n";
	int r = a + b;
	std::cout << "      Res = " << r << "\n";
	return r;
}

int fun2(){
	std::cout << "    fun2\n";
	
	return fun3(10, 2);
}

int fun1(){
	std::cout << "  fun1\n";
	return 1;
}

class Test {
public:
    Test(std::string name, std::function<void()> testFunc)
        : name_(std::move(name)), testFunc_(std::move(testFunc)) {}

    void run() const {
        std::cout << "Running test: " << name_ << std::endl;
        testFunc_();
    }

private:
    std::string name_;
    std::function<void()> testFunc_;
};

class TestSuite {
public:
    TestSuite(std::string name) : name_(std::move(name)) {}

    TestSuite& operator=(std::function<void(TestSuite&)> suiteFunc) {
        suiteFunc(*this);
        return *this;
    }

    void addTest(std::string name, std::function<void()> testFunc) {
        tests_.emplace_back(std::move(name), std::move(testFunc));
    }

    void run() const {
        std::cout << "Running suite: " << name_ << std::endl;
        for (const auto& test : tests_) {
            test.run();
        }
    }

private:
    std::string name_;
    std::vector<Test> tests_;
};

class TestRunner {
public:
    TestRunner& addSuite(TestSuite suite) {
        suites_.push_back(std::move(suite));
        return *this;
    }

    void run() const {
        for (const auto& suite : suites_) {
            suite.run();
        }
    }

private:
    std::vector<TestSuite> suites_;
};

// Usage example
TestRunner TEST;

auto suit1 = [](TestSuite& suite) {
    suite.addTest("Example Test 1.1", [] {
        std::cout << "This is test case 1.1" << std::endl;
		fun1();
    });

    suite.addTest("Example Test 1.2", [] {
        std::cout << "This is test case 1.2" << std::endl;
		fun2();
    });
};

auto suit2 = [](TestSuite& suite) {
    suite.addTest("Example Test 2.1", [] {
        std::cout << "This is test case 2.1" << std::endl;
		prc1();
    });

    suite.addTest("Example Test 2.2", [] {
        std::cout << "This is test case 2.2" << std::endl;
		prc2();
    });
};

int main() {
	//monstartup ((u_long) &_start, (u_long) &etext);
	
	std::cout << "main:\n";
	for(int i = 0; i < 1000; i++){
		fun1();
		fun2();
		fun3(1, 10);
		
		prc1();
		prc2();
		prc3();
	}


    TEST.addSuite(TestSuite("Suite 1") = suit1);

    TEST.addSuite(TestSuite("Suite 2") = suit2);

    TEST.run();
    return 0;
}
