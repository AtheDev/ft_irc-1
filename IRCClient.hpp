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
	UNREGISTERED, PASSWORD, NICK, REGISTERED
};

class IRCClient {
	public:

		IRCClient(int fd, std::string servername);
		~IRCClient();

		int get_fd() const;
		std::string const & get_nickname(void) const;
		std::string const & get_username(void) const;
		std::string const & get_hostname(void) const;
		std::string const & get_realname(void) const;
		std::string const & get_mode(void) const;
		std::string const & get_away_message(void) const;
		std::string const & get_servername(void) const;
		int get_status(void) const;
		std::string get_prefix() const;
		bool is_mode(char mode) const;

		void set_nickname(const std::string& nickname);
		void set_username(const std::string& username);
		void set_hostname(const std::string& username);
		void set_realname(const std::string& realname);
		void set_away_message(const std::string& message);
		void set_mode(char sign, char mode);
		void set_status(int status);

	private:

		int _fd;
		int _status;

		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string	_mode;
		std::string _hostname;
		std::string	_away_message;
		std::string _servername;

		IRCClient();
		IRCClient(IRCClient const & cpy);
		IRCClient & operator=(IRCClient const & rhs);

};

#endif