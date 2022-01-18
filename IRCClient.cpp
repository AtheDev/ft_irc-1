#include "IRCClient.hpp"


IRCClient::IRCClient(int fd) :
		_fd(fd), _status(UNREGISTERED), _nickname(), _username(), _realname(), _password(),
		_mode(""), _hostname(), _away_message(), _channels() {
}

IRCClient::~IRCClient() {}


int IRCClient::get_fd() const { return _fd; }

std::string const & IRCClient::get_nickname(void) const { return _nickname; }

std::string const & IRCClient::get_username(void) const { return _username; }

std::string const & IRCClient::get_hostname(void) const { return _hostname; }

std::string const & IRCClient::get_realname(void) const { return _realname; }

std::string const & IRCClient::get_mode(void) const { return _mode; }

std::string const & IRCClient::get_away_message(void) const { return _away_message; }

std::vector<std::string> IRCClient::get_channels(void) const { return _channels; }

int IRCClient::get_status(void) const { return _status; }

std::string IRCClient::get_prefix() const {
	return std::string(_nickname + "!" + _username + "@" + _hostname);
}


void IRCClient::set_nickname(std::string nickname) { _nickname = nickname; }

void IRCClient::set_username(std::string username) { _username = username; }

void IRCClient::set_hostname(std::string hostname) { _hostname = hostname; }

void IRCClient::set_realname(std::string realname) { _realname = realname; }

void IRCClient::set_password(std::string password) { _password = password; }

void IRCClient::set_away_message(std::string message) { _away_message = message; }

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

bool IRCClient::is_visible(void) const {
		if (_mode.find('i') != std::string::npos)
			return false;
	return true;
}

void IRCClient::join_channel(std::string channel) {

	if (is_in_channel(channel) == false) {
		_channels.push_back(channel);
	}
}

void IRCClient::quit_channel(std::string channel) {

	if (is_in_channel(channel) == true) {
		_channels.erase(find(get_channels().begin(), get_channels().end(), channel));
	}
}

bool IRCClient::is_in_channel(std::string channel) const {

	if (find(get_channels().begin(), get_channels().end(), channel) == get_channels().end()) {
		return false;
	}
	return true;
}
