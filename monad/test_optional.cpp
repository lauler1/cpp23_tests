#include <iostream>
#include <optional>
#include <functional>


std::optional<int> safe_divide(int numerator, int denominator) {
    std::cout << "safe_divide\n";
    if (denominator == 0) return std::nullopt; // Error case
    return numerator / denominator; // Success case
}

std::optional<int> add_five(int x) {
    std::cout << "add_five\n";
    return x + 5;
}

int add_two(int x) {
    std::cout << "add_two\n";
    return x + 2;
}

std::optional<int> orelse() {
    std::cout << "orelse\n";
    return 5;
}

int main() {
    // Successful case:
    std::optional<int> result = safe_divide(10, 2)
        .and_then(add_five)
        .and_then(add_five) // Chaining operations successfully
        .transform(add_two) // Chaining operations successfully
        .or_else(orelse); // Chaining operations successfully

    if (result) {
        std::cout << *result << std::endl; // Correct output: 15
    }

    // Error case:
    std::optional<int> error_result = safe_divide(10, 0)
        .and_then(add_five)
        .and_then(add_five) // Chaining halts due to division by zero
        .transform(add_two) // Chaining operations successfully
        .or_else(orelse); // Chaining operations successfully

    if (error_result) {
        std::cout << *error_result << std::endl;
    } else {
        std::cout << "Error: Division by zero." << std::endl; // Error handling
    }

    return 0;
}
