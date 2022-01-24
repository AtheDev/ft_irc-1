#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <sys/socket.h>
#include <netdb.h>

#define USER_MODES() ("aiwroO")
#define MAX_CHANNELS 10

enum e_user_status {
	UNREGISTERED, PASSWORD, NICKNAME, REGISTERED
};

class IRCClient {
	public:

		IRCClient(int fd);
		~IRCClient();

		int get_fd() const;
		std::string const & get_nickname(void) const;
		std::string const & get_username(void) const;
		std::string const & get_hostname(void) const;
		std::string const & get_realname(void) const;
		std::string const & get_mode(void) const;
		std::string const & get_away_message(void) const;
		int get_status(void) const;
		std::string get_prefix() const;

		void set_nickname(std::string nickname);
		void set_username(std::string username);
		void set_hostname(std::string username);
		void set_realname(std::string realname);
		void set_password(std::string password);
		void set_away_message(std::string message);
		void set_mode(char sign, char mode);
		void set_status(int status);

	private:

		int _fd;
		int _status;

		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _password;
		std::string	_mode;
		//TODO: to be removed?
		std::string _hostname;
		std::string	_away_message;

		IRCClient();
		IRCClient(IRCClient const & cpy);
		IRCClient & operator=(IRCClient const & rhs);

};

#endif