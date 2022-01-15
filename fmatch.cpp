#include <string>
#include <cstring>
#include <cstdlib>
#include <climits>
#include <utility>

#define MIN 0
#define MAX 1

#define CHAR_MASK_SIZE 255

#include <cstdio> //for printf, debug
#include <iostream> //for std::cout, debug

/**
 * @brief
 * This is just for debug purposes
 */
static void
	display_char_mask(bool char_mask[CHAR_MASK_SIZE])
{
	std::cout << '|';
	for (int i = 0; i < CHAR_MASK_SIZE; ++i)
	{
		if (char_mask[i])
			printf("%c", i);
	}
	std::cout << '|' << std::endl;
}

static bool
	is_digit(int c) { return c >= '0' && c <= '9'; }

void
	remove_percentage(std::string &str)
{
	for (int i = 0; i < 2; ++i)
	{
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
		{
			if (*it == '%')
			{
				str.erase(it);
				break; //erase invalidates iterators
			}
		}
	}
}

/**
 * @brief
 * read a char or hex char at pos in the string s
 * 
 * @return
 * a pair with the first element being the amount of increment needed to continue reading the string
 * and the second element being the char read
 */
static std::pair<size_t, int>
	get_char(std::string s, size_t pos = 0)
{
	if (s[pos] == '%' && s[pos + 1] == '0') //get hex char
	{
		int	c = strtol(s.substr(pos + 1, 4).c_str(), NULL, 16);
		return std::pair<size_t, int>(5, c);
	}
	else //get plain char
		return std::pair<size_t, int>(1, s[pos]);
}

/**
 * @brief
 * Assign the amount of repetitions to the param repetition[2] by reading the string s
 * its a helper function of fmatch
 * 
 * @param s
 * a substr of a string being the content between the parenthesis indicating the amount of repetitions
 * for a char, range of char or pattern -see fmatch-
 * 
 * @return
 * the amount of increment needed to continue reading the string
 */
static size_t
	get_repetition(std::string s, int repetition[2])
{
	if (s.find(':') == s.npos) //if the amount of repetition is a single number
	{
		repetition[MIN] = atoi(s.c_str());
		repetition[MAX] = repetition[MIN];
	}
	else if (s == ":") //any amount of repetitions
	{
		repetition[MIN] = 0;
		repetition[MAX] = INT_MAX;
	}
	else if (sscanf(s.c_str(), "%d:%d", &repetition[MIN], &repetition[MAX]) == 2)
		;
	else if (sscanf(s.c_str(), "%d:", &repetition[MIN]) == 1)
		repetition[MAX] = INT_MAX;
	else if (sscanf(s.c_str(), ":%d", &repetition[MAX]) == 1)
		repetition[MIN] = 0;
	return s.size() + 2;
}

/**
 * @brief
 * Edits the char_mask to be true for values given by the string s
 * 
 * @param s
 * A substring of the content between the brackets formatted like described in fmatch
 * can also be a single char or hex char
 * 
 * @example
 * "a+b+c" as s will set the elements 97, 98 and 99 of the char_mask to 1
 * "a:z" as s will set all elements between 97 and 122 of the char_mask to 1
 */
static size_t
	fadd_char_mask(std::string s, bool char_mask[255])
{
	char					char_a, char_b;
	int						int_a, int_b;
	int						min, max;
	std::pair<size_t, int>	char_read;
	size_t					pos_x;
	size_t					r;

	if (s.find(':') == s.npos) //only one char to add to the mask
	{
		char_read = get_char(s);
		char_mask[char_read.second] = true; 
		return char_read.first;
	}
	r = s.size() + 2;
	remove_percentage(s); //removing the % in the string to allow using sscanf
	if ((pos_x = s.find('x')) != s.npos) //at least one hex char
	{
		if (s.find('x', pos_x + 1) != s.npos) //two hex char
		{
			sscanf(s.c_str(), "%x:%x", &int_a, &int_b);
			min = int_a;
			max = int_b;
		}
		else if (s.find(':', pos_x + 1) != s.npos) //one hex plain char then one char
		{
			sscanf(s.c_str(), "%x:%c", &int_a, &char_b);
			min = int_a;
			max = static_cast<int>(char_b);
		}
		else //one plain char then one hex char
		{
			sscanf(s.c_str(), "%c:%x", &char_a, &int_b);
			min = static_cast<int>(char_a);
			max = int_b;
		}
	}
	else //two plain char
	{
		sscanf(s.c_str(), "%c:%c", &char_a, &char_b);
		min = static_cast<int>(char_a);
		max = static_cast<int>(char_b);
	}
	for (int i = min; i <= max; ++i)
		char_mask[i] = true;
	return r;
}

/**
 * @brief
 * subpattern matching recursive function
 */
static bool
	fmatch_subpattern(std::string &token, std::string &format, size_t &pos_token, size_t &pos_format)
{
	bool	char_mask[CHAR_MASK_SIZE];
	int		n_repetition;
	int		repetition[2];

	memset(char_mask, false, CHAR_MASK_SIZE);
	++pos_format;
	if (format[pos_format] == '(')
		pos_format += get_repetition(format.substr(pos_format + 1, format.find(')', pos_format) - (pos_format + 1)), repetition);
	else
	{
		repetition[MIN] = 1;
		repetition[MAX] = 1;
	}
	do //editing the char_mask
	{
		if (format[pos_format] == '+')
			++pos_format;
		if (format[pos_format] == '(')
			; //recursion and break? pos_token is gonna mess everything up because its a ref
		if (format[pos_format] == '[')
			pos_format += fadd_char_mask(format.substr(pos_format + 1, format.find(']', pos_format) - (pos_format + 1)), char_mask);
		else if (format[pos_format] == '%') //hex char
			pos_format += fadd_char_mask(format.substr(pos_format, 5), char_mask);
		else
			pos_format += fadd_char_mask(format.substr(pos_format, 1), char_mask);
	} while (format[pos_format] == '+'); //extra condition for recursion?
	n_repetition = 0;
	while (n_repetition < repetition[MAX] && token[pos_token])
	{
		if (!char_mask[token[pos_token]])
			break ;
		++pos_token;
		++n_repetition;
	}
	return n_repetition >= repetition[MIN];
}

/**
 * @brief
 * Check if the string token matches the string format
 * 
 * @details
 * % indicates the start of a subpattern to match like this:
 * %(<a>:<b>)[<c>:<d>]
 * <a> and <b> are the minimum and maximum amount of char to match
 * there are numbers
 * omitting <a> is equivalent to a minimum of 0
 * omitting <b> is equivalent to maximum of INT_MAX
 * so to match any amount of char write (:)
 * to match exactly x char (x:x) is equivalent to (x)
 * if the amount of char to match is omitted it will match exactly one char
 * <c> and <d> are the bounds of the range of char to match
 * there are wether plain char or the hexadecimal value of the char like this: %0x00
 * the x MUST be lowercase
 * [<e>:<e>] can also be written as <e> to just match the char <e>
 * extra char or range of char can be added by separating them with +
 * you can specify a different amount of times to match the extra char or range of char
 * you can specify other possible subpattern to match by specifying a new min and max after the +
 * so that %(3)[a-z]+(1)[0-9] will match 3 letters or 1 number
 * 
 * * indicates the start of a pattern to match like:
 * *((<a>:<b>)[<pattern>])
 * <a> and <b> are the minimum and maximum amount of patterns to match
 * they follow the same rules as the <a> and <b> for subpatterns
 * pattern can be any sequence of plain chars or %(...) separated by nothing
 * you can add extra patterns to match by separating them with +
 * you can specify a different amount of times to match the extra patterns
 * you can specify extra possible pattern to match you like for subpatterns
 * you can match repeating pattern(s) inside a repeating pattern
 * 
 * a single plain char (except % or *) will match one time this single plain char
 * 
 * @example
 * "a" will match a single a char
 * "%[a:z]" will match a single lowercase letter
 * "%(4:6)[0:9][A:Z]" will match between 4 and 6 numbers or capital letters
 * "*((3)[%(1:3)[0:9].]+(1)X)%(1:3)[0:9]+(1)X"
 * will match 3 times a pattern of between one and 3 numbers or a single X followed by a .
 * and then between one and 3 numbers or a single X
 * so it will match 127.0.0.1 or 41.X.X.0 or X.X.X.X
 * 
 * @note
 * % cannot be used as a plain char, you must use %0x25
 * * cannot be used as a plain char, you must use %0x2A
 * + cannot be used as a plain char, you must use %0x2B
 */
bool
	fmatch(std::string token, std::string format)
{
	size_t					pos_token = 0, pos_format = 0;
	std::pair<size_t, int>	char_read;

	while (format[pos_format] && token[pos_token])
	{
		if (format[pos_format] == '*') //matching pattern(s)
		{

		}
		else if (format[pos_format] == '%' && format[pos_format + 1] != '0') //matching subpattern(s)
		{
			if (!fmatch_subpattern(token, format, pos_token, pos_format))
				return false;
		}
		else //matching a single plain char or hex char
		{
			char_read = get_char(format, pos_format);
			pos_format += char_read.first;
			if (token[pos_token] != char_read.second)
				return false;
			++pos_token;
		}
	}
	if (format[pos_format])
		; //check if all remaining patterns and subptatterns accept 0 repetition
	return !format[pos_format] && !token[pos_token];
}