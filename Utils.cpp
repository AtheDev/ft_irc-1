#include "Utils.hpp"

std::string to_string(size_t number) {

    std::string str;
    std::string tmp;

    if (number == 0)
        str = "0";
    while (number > 0)
    {
        tmp[0] = number % 10 + 48;
        tmp += str;
        str = tmp;
        number /= 10;
    }
    return str;
}