#include "IRCClient.hpp"

IRCClient::IRCClient(int fd, struct sockaddr_in addr): _fd(fd), _nickname(),
    _username(), _realname(),_mode(), _hostname(), _password(), _channels() {

    char host[NI_MAXHOST];
    if (getnameinfo((struct sockaddr *)&addr, sizeof(addr), host, NI_MAXHOST, NULL, 0, 0) != 0)
        std::cout << "Error getnameinfo" << std::endl;
    else
        _hostname = host;
}

IRCClient::~IRCClient() {}

std::string const &                 IRCClient::get_nickname(void) const { return _nickname;}

std::string const &                 IRCClient::get_username(void) const { return _username;}

std::string const &                 IRCClient::get_realname(void) const {return _realname;}

std::string const &                 IRCClient::get_mode(void) const { return _mode;}

std::vector<std::string> const &    IRCClient::get_channels(void) const { return _channels;}

void    IRCClient::set_nickname(std::string nickname) { _nickname = nickname;}

void    IRCClient::set_username(std::string username) { _username = username;}

void    IRCClient::set_realname(std::string realname) { _realname = realname;}

void    IRCClient::set_mode(std::string mode) { _mode = mode;}

void    IRCClient::set_channels(std::string channel) { _channels.push_back(channel);}

void    IRCClient::set_password(std::string password) { _password = password;}

bool    IRCClient::_is_in_this_channel(std::string channel) const {

    std::vector<std::string>::const_iterator it = get_channels().begin();
    for (; it != get_channels().end(); it++)
        if (*it == channel)
            return true;
    return false;
}