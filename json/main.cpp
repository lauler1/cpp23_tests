#include <any>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <iterator>
#include <array>
extern void printAny(const std::any& value, const std::string& indent = "");

void printValue(const std::any& value, const std::string& indent = ""){
//std::cout << value.type().name() << ":";

    if (value.type() == typeid(std::nullptr_t)) {
        std::cout << "null";
    }
    else if (value.type() == typeid(int)) {
        std::cout << std::any_cast<int>(value);
    }
    else if (value.type() == typeid(bool)) {
        std::cout << std::any_cast<bool>(value);
    }
    else if (value.type() == typeid(float)) {
        std::cout << std::any_cast<double>(value);
    }
    else if (value.type() == typeid(double)) {
        std::cout << std::any_cast<double>(value);
    }
    else if (value.type() == typeid(std::string)) {
        std::cout << "\"" << std::any_cast<std::string>(value) << "\"";
    }
    else if (value.type() == typeid(const char*)) {
        std::cout << "\"" << std::any_cast<const char*>(value) << "\"";
    }
    else if (value.type() == typeid(std::map<std::string, std::any>)) {
        printAny(value, indent + "  ");
    }
    else {
        std::cout << "Unknown type:"  << value.type().name() <<"\n";
    }
}

void printAny(const std::any& value, const std::string& indent) {
//std::cout << value.type().name() << ":";
    if (value.type() == typeid(std::vector<std::any>)) {
        std::cout << "[";
        bool first = true;
        for (const auto& i : std::any_cast<std::vector<std::any>>(value)) {
            if(!first) std::cout << ", ";
            printValue(i, indent + "  ");
            first = false;
        }/**/
        std::cout << "]";
    }
    else if (value.type() == typeid(std::map<std::string, std::any>)) {
        std::cout << "{\n";
        bool first = true;
        for (const auto& [key, val] : std::any_cast<std::map<std::string, std::any>>(value)) {
            if(!first) std::cout << ",\n";
            std::cout << indent << "\"" << key << "\": ";
            printAny(val, indent);
            first = false;
        }
        std::cout << "\n" << indent << "}";
    }
    else {
        printValue(value);
    }
}

int main() {

    std::map<std::string, std::any> inmap = std::map<std::string, std::any>{{"asd1", "asd"},{"asd2", "asd"}};

    std::cout << std::boolalpha; //print bool as "true" and "false", not number
    std::map<std::string, std::any> myMap= {
        {"key1", 10},
        {"key2", "Hello, world"},
        {"key3", std::vector<std::any>{1.2, 2, 3, 4, 5, "sdf", std::map<std::string, std::any>{{"asd3", "asd"},{"asd4", "asd"}}, inmap }},
        {"key4", std::map<std::string, std::any>{
                {"inner1_key1", 42},
                {"inner1_key2", std::string("Hello, inner map")},
                {"inner1_key3", std::map<std::string, std::any>{
                    {"inner2_key1", 345.234},
                    {"inner2_key2", std::string("Hello, inner map")}
                    }
                }
            }
        },
        {"key5", true},
        {"key6", nullptr},
    };

    // Print the entire map structure
    printAny(myMap, "  ");

    return 0;
}
