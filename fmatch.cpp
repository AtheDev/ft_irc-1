#include <string>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <iostream>

static int get_char(std::string s, int pos)
{
	std::string tmp;

	if (s[pos] == '%')
	{
		tmp = s.substr(pos + 1, s.find('%', pos + 1) - 1);
		return atoi(tmp.c_str());
	}
	else
		return s[pos];
}

/**
 * @brief Check if the string token matches the string format
 * 
 * @details
 * format will match like:
 * %(1-2) -> 1 to 2 char -must be combined with plain char or range, see examples-
 * [0-9] -> range of char from 0 to 9 in ASCII table
 * + -> use this to add more range or plain char to match, see examples
 * plain char -> the char itself
 * char can also be written as %42% to match their decimal equivalent in ascii table
 * thus %(-)[a-z] is equivalent to %(-)[%97%-%122%]
 * this way you can add char that are not easily writable in the code
 * 
 * @example
 * %(1-3)[a-z] -> matches 1 to 3 char from a to z
 * %(1-)[0-9] -> matches 1 to infinity char from 0 to 9
 * %(8)H -> matches 8 H ("HHHHHHHH")
 * %(3)[0-9].%(3)[0-9]+(-)[a-z] -> matches 3 numbers then a . then 3 numbers then a + then any number of letters from a to z
 * %(-)[a-z]+[0-9] -> matches any number of letters or numbers
 * %(1-1)B+C+D+E -> matches exactly one char which is B, C, D or E
 * %37% -> maches one %
 * 
 * @note
 * It's gonna segfault if you don't respect the format so beware
 */
bool
	fmatch(std::string token, std::string format)
{
	size_t					pos_token = 0, pos_format = 0;
	std::string				tmp;
	int						n_to_match_min, n_to_match_max, n_matched;
	char					to_match[127], to_match1, to_match2;

	while (format[pos_format] && token[pos_token])
	{
		memset(to_match, 0, 127);
		if (format[pos_format] == '%' && format[pos_format + 1] == '(') //range of char
		{
			//here we get the amount of char to match
			pos_format += 2;
			if (format[pos_format] != '-')
			{
				tmp = format.substr(pos_format, format.find('-', pos_format) - pos_format);
				pos_format += tmp.size() + 1;
				n_to_match_min = atoi(tmp.c_str());
			}
			else
			{
				n_to_match_min = 0;
				++pos_format;
			}
			if (format[pos_format] != ')')
			{
				tmp = format.substr(pos_format, format.find(')', pos_format) - pos_format);
				pos_format += tmp.size() + 1;
				n_to_match_max = atoi(tmp.c_str());
			}
			else
			{
				n_to_match_max = INT_MAX;
				++pos_format;
			}
			//std::cout << '(' << n_to_match_min << '-' << n_to_match_max << ')' << std::endl; //DEBUG
			//here we get the range(s) of char to match
			do
			{
				if (format[pos_format] == '+')
					++pos_format;
				if (format[pos_format] == '[')
				{
					++pos_format;
					to_match1 = get_char(format, pos_format);
					if (format[pos_format] == '%')
						while (format[++pos_format] != '%')
							;
					pos_format += 2;
					to_match2 = get_char(format, pos_format);
					if (format[pos_format] == '%')
						while (format[++pos_format] != '%')
							;
					pos_format += 2;
					for (int i = to_match1; i <= to_match2; ++i)
						to_match[i] = 1;
				}
				else
				{
					to_match[get_char(format, pos_format)] = 1;
					if (format[pos_format] == '%')
						while (format[++pos_format] != '%')
							;
					++pos_format;
				}
			} while (format[pos_format] == '+');
		}
		else //just one char to match
		{
			n_to_match_min = 1;
			n_to_match_max = 1;
			to_match[get_char(format, pos_format)] = 1;
			if (format[pos_format] == '%')
				while (format[++pos_format] != '%')
					;
			++pos_format;
		}
		//for (int i = 0; i < 127; ++i) //DEBUG
		//{ //DEBUG
		//	if (to_match[i]) //DEBUG
		//		std::printf("%c", i); //DEBUG (might look weird with non-printable char)
		//} //DEBUG
		//std::cout << std::endl; //DEBUG
		//here we check if the token matches the format
		n_matched = 0;
		while (to_match[token[pos_token]] && n_matched < n_to_match_max)
		{
			++n_matched;
			++pos_token;
			if (!token[pos_token])
				break ;
		}
		if (n_matched < n_to_match_min)
			return false;
	}
	return !token[pos_token] && !format[pos_format];
}