#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

# include <vector>
# include <string>
# include <iostream>

class IRCMessage
{
	public: 

		IRCMessage(std::string line);
		~IRCMessage();
		bool empty() const;
		bool has_prefix() const;
		std::string get_prefix() const;
		std::string get_command() const;
		std::vector<std::string> get_params() const;

        void set_command(std::string cmd);
        

		class Error_message_empty : public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class Error_message_nocrlf : public std::exception {
			public:
				virtual const char *what() const throw();
		};
		class Error_message_invalid_prefix : public std::exception {
			public:
				virtual const char *what() const throw();
		};

	protected:

	private:

		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;

};

std::ostream &operator<<(std::ostream &o, const IRCMessage &i);

#endif