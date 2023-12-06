This example shows hot to cast from `const unsigned char uvar[]` to `std::array<char, sizeof(uvar) / sizeof(uvar[0])>` using `std::bit_cast` from `#include <bit>` lib.

This is safe but not so cheap.

C++23 has anew way much ceaper called `std::start_lifetime_as` and `std::start_lifetime_as_array` from `#include <memory>` lib. However not yet available at the time of this code was written.
