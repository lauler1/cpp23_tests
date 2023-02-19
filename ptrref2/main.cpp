#include "stacktrace.h"
#include <cassert>
#include <map>

struct Y
{
	int id_;
	Y(): id_{0} { std::cout << "Y: Deafault constructor\n";}
	Y(int id): id_{id}{ std::cout << "Y: constructor " << id << "\n";}
	Y(const Y &y){ std::cout << "Y: Copy constructor " << y.id_ << "\n"; }
	Y(Y &&y) noexcept { std::cout << "Y: Move constructor " << y.id_ << "\n"; }
	~Y(){ std::cout << "Y: Deafault destructor of "<< id_ << "\n"; id_ = 0;}
};


namespace Nu {

	void func3(auto y_f2){
		std::cout << "        func3 y_f2 = " << y_f2->id_ << std::endl;
	}

	auto func2(int val){
		auto y_f2 = std::make_unique<Y>(2);
		std::cout << "      func2 ptr y_f2 = " << y_f2.get() << std::endl;
		func3(std::move(y_f2));
		//auto y_f3 = std::move(y_f2);
		std::cout << "      func2 ptr y_f2 = " << y_f2.get() << std::endl;

		//std::cout << "      func2 val y_f2 = " << y_f2->id_ << " ptr y_f2= " << y_f2 << " y_f2.get() = " << y_f2.get() << std::endl;
		return y_f2; //With i works, with i.get() does not
	}

	void func1(void){
		auto y_f2 = func2(33);
		std::cout << "    func1 ptr y_f2 = " << y_f2 << std::endl;
		//std::cout << "    func1 val y_f2 = " << y_f2->id_ << std::endl;
	}

}//Nu

int main()
{
	std::cout << "Begin" << std::endl;
	try{
		
		Nu::func1();

		std::cout << "\nSecond part" << std::endl;
		auto y1 = std::make_unique<Y>(1);
		assert(y1.get() != nullptr);
		std::cout << y1.get() << std::endl;
		auto y2 = std::move(y1);
        std::cout << y1.get() << " " << y2.get() << std::endl;		
		
		//throw std::runtime_error("Throw test");

	}catch(std::exception &e){
		std::cout << "Error = " << typeid(e).name() << ": " << e.what() << std::endl;
		DBG::print_stacktrace();
	}catch(...){
		std::cout << "Error: " << std::endl;
		DBG::print_stacktrace();
	}
	
	std::cout << "\nThird part" << std::endl;
	
	std::cout << "y3 Declar"  << std::endl;
	{
	std::unique_ptr<Y> y3;
		std::cout << "y3 Construct"  << std::endl;

		y3 = std::make_unique<Y>(3);
		
		std::cout << "y3 Leave context"  << std::endl;
	}
	
	  
	std::cout << "End" << std::endl;
	
     // This will produce an error at compile time
     //if int size greater than 16
    _Static_assert(sizeof(int) == 4, "Expecting 16 bit integers");
}

/*
Output

#With func2 --> return return y_f2.get()
Begin
Y: constructor 2
      func2 ptr y_f2 = 0x56222a4f22c0
      func2 ptr y_f2 = 0x56222a4f22c0
Y: Deafault destructor of 2
    func1 ptr y_f2 = 0x56222a4f22c0

Second part
Y: constructor 1
0x56222a4f22c0
0 0x56222a4f22c0

Y: Deafault destructor of 1

Third part
y3 Declar
y3 Construct
Y: constructor 3
y3 Leave context
Y: Deafault destructor of 3
End


#With func2 --> return return y_f2
Begin
Y: constructor 2
      func2 ptr y_f2 = 0x563c9bf2f2c0
      func2 ptr y_f2 = 0x563c9bf2f2c0
    func1 ptr y_f2 = 0x563c9bf2f2c0
Y: Deafault destructor of 2

Second part
Y: constructor 1
0x563c9bf2f2c0
0 0x563c9bf2f2c0
Y: Deafault destructor of 1

Third part
y3 Declar
y3 Construct
Y: constructor 3
y3 Leave context
Y: Deafault destructor of 3
End
*/