#include <inpututil.hh>
#include <iostream>

std::string inputf(std::string message) {
    std::string out;
    std::cout << message;
    std::getline(std::cin, out);
    return out;
}
