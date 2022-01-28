#include "IRCClient.hpp"


IRCClient::IRCClient(int fd, std::string servername) :
		_fd(fd), _status(UNREGISTERED), _nickname(), _username(), _realname(),
		_mode(""), _hostname(), _away_message(), _servername(servername) {
}

IRCClient::~IRCClient() {}


int IRCClient::get_fd() const { return _fd; }

std::string const & IRCClient::get_nickname(void) const { return _nickname; }

std::string const & IRCClient::get_username(void) const { return _username; }

std::string const & IRCClient::get_hostname(void) const { return _hostname; }

std::string const & IRCClient::get_realname(void) const { return _realname; }

std::string const & IRCClient::get_mode(void) const { return _mode; }

std::string const & IRCClient::get_away_message(void) const { return _away_message; }

std::string const & IRCClient::get_servername(void) const { return _servername; }

int IRCClient::get_status(void) const { return _status; }

std::string IRCClient::get_prefix() const {
	std::string prefix;
	if (get_nickname().empty())
		prefix = "*!";
	else
		prefix = _nickname + "!";
	if (get_username().empty())
		prefix += "*@";
	else
		prefix += _username + "@";
	if (get_hostname().empty())
		prefix += "*";
	else
		prefix += _hostname;
	return prefix;
}

bool IRCClient::is_mode(char mode) const {
	return _mode.find(mode) != std::string::npos;
}

void IRCClient::set_nickname(const std::string& nickname) { _nickname = nickname; }

void IRCClient::set_username(const std::string& username) { _username = username; }

void IRCClient::set_hostname(const std::string& hostname) { _hostname = hostname; }

void IRCClient::set_realname(const std::string& realname) { _realname = realname; }

void IRCClient::set_away_message(const std::string& message) { _away_message = message; }

void IRCClient::set_status(int status) { _status = status; }

void IRCClient::set_mode(char sign, char mode) {
	if (sign == '+')
	{
		if (_mode.find(mode) == std::string::npos)
			_mode.push_back(mode);
	}
	else
	{
		if (_mode.find(mode) != std::string::npos)
			_mode.erase(_mode.find(mode));
	}
}
