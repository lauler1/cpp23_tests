#include <bit>
#include <iostream>
#include <array>

const unsigned char uvar[] = "abcdefgh";

template <typename T, std::size_t N>
constexpr size_t calc_size(T (&)[N]) {
    return N;
}
constexpr  size_t size = sizeof(uvar) / sizeof(uvar[0]);

int main() {
    // Convert unsigned char array to std::array<char, size>
    std::array<char, sizeof(uvar) / sizeof(uvar[0])> charArray = std::bit_cast<std::array<char, sizeof(uvar) / sizeof(uvar[0])>>(uvar);
    //std::array<char, calc_size(uvar)> charArray = std::bit_cast<std::array<char, calc_size(uvar)>>(uvar);//Alternative with constexpr array_size()
    //std::array<char, size> charArray = std::bit_cast<std::array<char, size>>(uvar);                        //Alternative with constexpr size
    //std::array<char, 9> charArray = std::bit_cast<std::array<char, 9>>(uvar);                              //Alternative with manual size calc

    std::cout << "Char Array: " << charArray.data() << std::endl;
    std::cout << "array_size: " << calc_size(uvar) << std::endl;
    std::cout << "size: " << size << std::endl;

    //std::string str{charArray.data(), charArray.size()};
    std::string_view str{charArray.data(), charArray.size()};
    std::cout << "str: " << str << std::endl;
    return 0;
}
