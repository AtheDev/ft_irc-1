#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <string>
#include <algorithm>
#include <vector>
#include <sys/socket.h>
#include <netdb.h>


enum e_user_mode {

	MODE_A = (1U << 1),
	MODE_I = (1U << 2),
	MODE_W = (1U << 3),
	MODE_O_MIN = (1U << 4),
	MODE_O_MAJ = (1U << 5),
	MODE_S = (1U << 6)
};

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
		short const & get_mode(void) const;
		std::vector<std::string> get_channels(void) const;
		int get_status(void) const;
		std::string get_prefix() const;

		void set_nickname(std::string nickname);
		void set_username(std::string username);
		void set_hostname(std::string username);
		void set_realname(std::string realname);
		void set_password(std::string password);
		void set_mode(char sign, char flag);
		void set_status(int status);

		void join_channel(std::string channel);
		void quit_channel(std::string channel);
		bool is_in_channel(std::string channel) const;


	private:

		int _fd;
		int _status;

		std::string _nickname;
		std::string _username;
		std::string _realname;
		std::string _password;
		short _mode;
		//TODO: to be removed?
		std::string _hostname;

		std::vector<std::string> _channels;

		IRCClient();
		IRCClient(IRCClient const & cpy);
		IRCClient & operator=(IRCClient const & rhs);

};

#endif