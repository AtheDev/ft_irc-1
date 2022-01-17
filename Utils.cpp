#include "Utils.hpp"

std::string to_string(size_t number) {

    std::string str;
    char tmp[2];
    tmp[1] = '\0';
    if (number == 0)
        str = "0";
    while (number > 0)
    {
        tmp[0] = number % 10 + 48;
        str.insert(0, std::string(tmp));
        number /= 10;
    }
    return str;
}