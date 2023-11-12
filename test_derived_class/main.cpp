#include <concepts>
#include <type_traits>
 
class A {};
 
class B: public A {};
 
class C: private A {};


class D: public B {};

class E {};

template <typename T>
requires std::derived_from<T, A>
void func([[maybe_unused]] T x){

}

int main()
{
    // std::derived_from == true only for public inheritance or exact same class
    static_assert( std::derived_from<B, B> == true );      // same class: true
    static_assert( std::derived_from<int, int> == false ); // same primitive type: false
    static_assert( std::derived_from<B, A> == true );      // public inheritance: true
    static_assert( std::derived_from<C, A> == false );     // private inheritance: false
    static_assert( std::derived_from<D, B> == true);     // public inheritance: false
 
    // std::is_base_of == true also for private inheritance
    static_assert( std::is_base_of_v<B, B> == true );      // same class: true
    static_assert( std::is_base_of_v<int, int> == false ); // same primitive type: false
    static_assert( std::is_base_of_v<A, B> == true );      // public inheritance: true
    static_assert( std::is_base_of_v<A, C> == true );      // private inheritance: true
    static_assert( std::is_base_of_v<A, D> == true , "blablabla");      // public inheritance: true

    D d{};
    func(d);
    [[maybe_unused]] E e{};
    //func(e); //Will fail because of concept requirement

    return 0;
}
