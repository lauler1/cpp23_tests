#include <iostream>
#include <expected>
#include <functional>
#include <any>
#include <utility>

enum class parse_error{f1, f2,f3};

std::expected<std::any, parse_error> add_five(std::any x) {
    std::cout << ">> add_five\n";
    return std::any_cast<int>(x) + 5;
}

std::expected<std::any, parse_error> to_str(std::any x) {
    std::cout << ">> to_str\n";
    return "My String = "+std::to_string(std::any_cast<int>(x));
}

std::expected<std::any, parse_error> make_error([[maybe_unused]] std::any x) {
    std::cout << " >> unexpected\n";
    return std::unexpected(parse_error::f3);

}

int main() {
    std::expected<std::any, parse_error> res = int{10};
    std::vector<std::function<std::expected<std::any, parse_error>(std::any)>> functions;
    functions.push_back(add_five);
    //functions.push_back(make_error);
    functions.push_back(to_str);

    for(const auto& func: functions){
        std::cout << "-->\n";
        res = res.and_then(func);
    }
    if (res) {
        std::cout << std::any_cast<std::string>(*res) << std::endl; // Correct output
    }else{
        std::cout << "Error: " << std::to_underlying(res.error()) << std::endl; // Error handling
    }
    return 0;
}
