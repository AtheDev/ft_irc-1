#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

# include <vector>
# include <string>
# include <iostream>
# include <cstdlib>
# include <climits>

# include <string>
# include "IRCServer.hpp"

enum	e_param_type {
	NONE
};

class IRCMessage
{
	public: 

		//IRCMessage(TCPMessage &tcpmessage);
		IRCMessage(std::string line);
		~IRCMessage();
		bool empty() const;
		bool has_prefix() const;
		std::string get_prefix() const;
		std::string get_command() const;
		std::vector<std::pair<int, std::string> > get_params() const;
		int get_sender(void) const;
		std::vector<int> get_receivers() const;

		TCPMessage to_tcp_message();
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

		int											_sender;
		std::string									_prefix;
		std::string									_command;
		std::vector<std::pair<int, std::string> >	_params;
		std::vector<int>							_receivers;

		void						_parse_line(std::string line);
		void						_sanity_check();

};

std::ostream &operator<<(std::ostream &o, const IRCMessage &i);

#endif