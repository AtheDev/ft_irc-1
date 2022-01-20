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

std::vector<std::string> ft_split(std::string s, std::string const & delimiter) {
	size_t pos;
	std::vector<std::string> tokens;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		tokens.push_back(s.substr(0, pos));
		s.erase(0, pos + delimiter.length());
	}
	tokens.push_back(s.substr(0, pos));
	return tokens;
}