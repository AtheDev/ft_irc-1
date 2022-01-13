#include <string>
#include <cstring>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <iostream>

static bool is_digit(int c)
{
	return c >= '0' && c <= '9';
}

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

static void assign_amount(std::string format, int pos, int *min, int *max)
{
	std::string tmp;

	//pos is right after the (
	if (is_digit(format[pos]) && (format.find('-', pos) == format.npos || format.find('-', pos) > format.find(')', pos)))
	{
		tmp = format.substr(pos, format.find(')', pos) - pos);
		pos += tmp.size() + 1;
		*min = atoi(tmp.c_str());
		*max = *min;
	}
	else
	{
		if (format[pos] != '-')
		{
			tmp = format.substr(pos, format.find('-', pos) - pos);
			pos += tmp.size() + 1;
			*min = atoi(tmp.c_str());
		}
		else
		{
			*min = 0;
			++pos;
		}
		if (format[pos] != ')')
		{
			tmp = format.substr(pos, format.find(')', pos) - pos);
			pos += tmp.size() + 1;
			*max = atoi(tmp.c_str());
		}
		else
		{
			*max = INT_MAX;
			++pos;
		}
	}
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
 * %R can be used to repeat a pattern -see examples-
 * 
 * @example
 * %(1-3)[a-z] -> matches 1 to 3 char from a to z
 * %(1-)[0-9] -> matches 1 to infinity char from 0 to 9
 * %(8)H -> matches 8 H ("HHHHHHHH")
 * %(3)[0-9].%(3)[0-9]+(-)[a-z] -> matches 3 numbers then a . then 3 numbers then a + then any number of letters from a to z
 * %(-)[a-z]+[0-9] -> matches any number of letters or numbers
 * %(1)B+C+D+E -> matches exactly one char which is B, C, D or E
 * equivalent to %(1-1)B+C+D+E
 * %37% -> maches one %
 * to match any number of repeating patterns:
 * %R(-).%(3)[a-z]%R -> match any number of the pattern . then 3 letters
 * 
 * @note
 * It's gonna segfault if you don't respect the format so beware
 */
bool
	fmatch(std::string token, std::string format)
{
	size_t			pos_token = 0, pos_format = 0, pos_pattern_repeat;
	std::string		tmp;
	int				char_to_match_min, char_to_match_max, char_matched;
	int				pattern_to_match_min, pattern_to_match_max, pattern_matched;
	bool			pattern;
	char			char_mask[255], char_to_match_lower_bound, char_to_match_upper_bound;

	while (format[pos_format] && token[pos_token])
	{
		if (format[pos_format] == '%' && format[pos_format + 1] == 'R') //range of pattern
		{
			pattern = true;
			//here we get the amount of pattern to match
			pos_format = format.find('(', pos_format) + 1;
			assign_amount(format, pos_format, &pattern_to_match_min, &pattern_to_match_max);
			pos_format = format.find(')', pos_format) + 1;
			pos_pattern_repeat = pos_format;
		}
		else
		{
			pattern = false;
			pattern_to_match_min = 1;
			pattern_to_match_max = 1;
			pos_pattern_repeat = pos_format;
		}
		pattern_matched = 0;
		while (pattern_matched < pattern_to_match_max)
		{
			memset(char_mask, 0, 255);
			if (format[pos_format] == '%' && format[pos_format + 1] == '(') //range of char
			{
				//here we get the amount of char to match
				pos_format = format.find('(', pos_format) + 1;
				assign_amount(format, pos_format, &char_to_match_min, &char_to_match_max);
				pos_format = format.find(')', pos_format) + 1;
				//std::cout << '(' << char_to_match_min << '-' << char_to_match_max << ')' << std::endl; //DEBUG
				//here we get the range(s) of char to match
				do
				{
					if (format[pos_format] == '+')
						++pos_format;
					if (format[pos_format] == '[')
					{
						++pos_format;
						char_to_match_lower_bound = get_char(format, pos_format);
						if (format[pos_format] == '%')
							while (format[++pos_format] != '%')
								;
						pos_format += 2;
						char_to_match_upper_bound = get_char(format, pos_format);
						if (format[pos_format] == '%')
							while (format[++pos_format] != '%')
								;
						pos_format += 2;
						for (int i = char_to_match_lower_bound; i <= char_to_match_upper_bound; ++i)
							char_mask[i] = 1;
					}
					else
					{
						char_mask[get_char(format, pos_format)] = 1;
						if (format[pos_format] == '%')
							pos_format = format.find('%', pos_format + 1);
						++pos_format;
					}
				} while (format[pos_format] == '+');
			}
			else //just one char to match
			{
				char_to_match_min = 1;
				char_to_match_max = 1;
				char_mask[get_char(format, pos_format)] = 1;
				if (format[pos_format] == '%' && is_digit(format[pos_format + 1]))
					pos_format = format.find('%', pos_format + 1);
				++pos_format;
			}
			//std::cout << '|';
			//for (int i = 0; i < 127; ++i) //DEBUG
			//{ //DEBUG
			//	if (char_mask[i]) //DEBUG
			//		std::printf("%c", i); //DEBUG (might look weird with non-printable char)
			//} //DEBUG
			//std::cout << '|' << std::endl; //DEBUG
			//here we check if the token matches the format
			char_matched = 0;
			while (char_mask[token[pos_token]] && char_matched < char_to_match_max)
			{
				++char_matched;
				++pos_token;
				if (!token[pos_token])
					break ;
			}
			if (char_matched < char_to_match_min)
				return false;
			if (!pattern)
				++pattern_matched;
			else if (format[pos_format] == '%' && format[pos_format + 1] == 'R')
			{
				++pattern_matched;
				if (!token[pos_token])
					break ;
				if (pattern_matched < pattern_to_match_max)
					pos_format = pos_pattern_repeat;
			}
		}
		if (pattern_matched < pattern_to_match_min)
			return false;
		if (pattern && format[pos_format] == '%' && format[pos_format + 1] == 'R')
			pos_format += 2;
	}
	return !token[pos_token] && !format[pos_format];
}