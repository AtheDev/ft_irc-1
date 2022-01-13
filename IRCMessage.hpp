#ifndef IRCMESSAGE_HPP
#define IRCMESSAGE_HPP

# include <vector>
# include <string>
# include <iostream>

# include "IRCServer.hpp"

//# define ERR_ALREADYREGISTERED() ("462 :Unauthorized command (already registered)")
//# define ERR_NICKNAMEINUSE(nick) ("433 " + nick + " :Nickname is already in use")
//# define ERR_NICKCOLLISION(nick, user, host) ("436 " + nick + " :Nickname collision KILL from " + user + "@host")

# define RPL_WELCOME(nick, user, host) ("001 :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
# define RPL_YOURHOST(servername, version) ("002 :Your host is " + servername + ", running version " + version)
# define RPL_CREATED(date) ("003 :This server was created " + date)
# define RPL_MYINFO(servername, version, user_modes, channel_modes) ("004 :" + servername + " " + version + " " + user_modes + " " + channel_modes)

//enum e_err_reply {
//	ERR_ALREADYREGISTERED = 401,
//	ERR_NICKNAMEINUSE,
//	ERR_NICKCOLLISION
//};
//
//enum e_rpl_reply {
//	RPL_WELCOME,
//	RPL_YOURHOST,
//	RPL_CREATED,
//	RPL_MYINFO
//};

class IRCServer;

class IRCMessage
{
	public: 

		IRCMessage(TCPMessage &tcpmessage);
		~IRCMessage();
		bool empty() const;
		bool has_prefix() const;
		std::string get_prefix() const;
		std::string get_command() const;
		std::vector<std::string> get_params() const;
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

		int							_sender;
		std::string					_prefix;
		std::string					_command;
		std::vector<std::string>	_params;
		std::vector<int>			_receivers;

		void						_parse_line(std::string line);

};

std::ostream &operator<<(std::ostream &o, const IRCMessage &i);

#endif