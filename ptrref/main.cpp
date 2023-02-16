#include "stacktrace.h"
#include <cassert>
#include <map>

struct Xstruct {};

// overloads. From https://bajamircea.github.io/coding/cpp/2016/04/07/move-forward.html
void fn(Xstruct &) { std::cout<< "X &\n"; }
void fn(const Xstruct &) { std::cout<< "const X &\n"; }
void fn(Xstruct &&) { std::cout<< "X &&\n"; }

template<typename T>
void fn2(T &&t) { std::cout<< "template " << DBG::print_demangle(t) <<"\n"; }

namespace Nu {

	void func4(auto i){
		std::cout << "        func4 i = " << *i << std::endl;
	}

	auto func3(int val){
		std::unique_ptr<int> i = std::make_unique<int>(55);
		std::cout << "      " << i.get() << std::endl;
		//func4(std::move(i));
		//auto qi = std::move(i);
		std::cout << "      " << i.get() << std::endl;

		std::cout << "      func3 val i = " << *i << " ptr i= " << i << " i.get() = " << i.get() << std::endl;
		return i.get(); //With i works, with i.get() does not
	}

	void func2(void){
		auto i = func3(33);
		std::cout << "    func2 ptr i = " << i << std::endl;
		std::cout << "    func2 val i = " << *i << std::endl;
	}

	void func1(void){
		func2();
	}

}//Nu


struct Y
{
  int id_;
  Y(): id_{0} { std::cout << "Y: Deafault constructor\n";}
  Y(int id): id_{id}{ std::cout << "Y: constructor " << id << "\n";}
  Y(const Y &y){ std::cout << "Y: Copy constructor " << y.id_ << "\n"; }
  Y(Y &&y) noexcept { std::cout << "Y: Move constructor " << y.id_ << "\n"; }
};

struct X
{
	Y a_;
	Y b_;

	X(){ std::cout << "X: Deafault constructor\n"; }

	template<typename A, typename B>
	X(A && a, B && b) :
		// retrieve the original value category from constructor call
		// and pass on to member variables
		a_{ std::forward<A>(a) },
		b_{ std::forward<B>(b) }
	{
		std::cout << "X: constructor (A && a, B && b)\n";
	}
};

template<typename A, typename B>
X factory(A && a, B && b)
{
	std::cout << "X: factory (A && a, B && b)\n";
	// retrieve the original value category from the factory call
	// and pass on to X constructor
	return X(std::forward<A>(a), std::forward<B>(b));
}

int main()
{
	std::cout << "Begin" << std::endl;
	try{
		auto pi = std::make_unique<int>(42);
		assert(pi.get() != nullptr);
		std::cout << pi.get() << std::endl;
		auto qi = std::move(pi);
        std::cout << pi.get() << " " << qi.get() << std::endl << std::endl;		
		Nu::func1();
		
		//throw std::runtime_error("timed out");

		const int ii = 2;
		std::unique_ptr<const int> i = std::make_unique<const int>(ii);


		//int *i = NULL;
		std::cout << "i = " << *i << std::endl;
		//assert(pi.get() == nullptr);
		//assert(qi.get() != nullptr);
		//assert(pi.get() != nullptr);
	}catch(std::exception &e){
		std::cout << "Error = " << typeid(e).name() << ": " << e.what() << std::endl;
		DBG::print_stacktrace();
	}catch(...){
		std::cout << "Error: " << std::endl;
		DBG::print_stacktrace();
	}
	
	std::cout << "\nPart 2" << std::endl;
	
	Xstruct a;
	fn(a);
	// lvalue selects fn(X &)
	// fallbacks on fn(const X &)

	const Xstruct b;
	fn(b);
	// const lvalue requires fn(const X &)

	fn(Xstruct());
	// rvalue selects fn(X &&)
	// and then on fn(const X &)
	
	std::cout << "\nPart 3" << std::endl;
	
	fn2(10);
	fn2(&a);


	fn2(a);
	// argument expression is lvalue of type X
	// resolves to T being X &
	// X & && collapses to X &

	fn2(Xstruct());
	// argument expression is rvalue of type X
	// resolves to T being X
	// X && stays X &&
	
	std::cout << "\nPart 4" << std::endl;
	
	Y y{1};
	X two = factory(y, Y(2));
	// the first argument is a lvalue, eventually a_ will have the
	// copy constructor called
	// the second argument is an rvalue, eventually b_ will have the
	// move constructor called
  
  
	std::cout << "End" << std::endl;

}

/*
Output
Begin
0x564e4711d2c0
0 0x564e4711d2c0

      0x564e4711d2e0
      0x564e4711d2e0
      func3 val i = 55 ptr i= 0x564e4711d2e0 i.get() = 0x564e4711d2e0
    func2 ptr i = 0x564e4711d2e0
    func2 val i = 1692692765
i = 2

Part 2
X &
const X &
X &&

Part 3
template int
template Xstruct*
template Xstruct
template Xstruct

Part 4
T: Deafault constructor
T: Deafault constructor
T: Copy constructor
T: Move constructor
End
*/