#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netdb.h>

class IRCClient {
    public:

        IRCClient(int fd, struct sockaddr_in addr);
        ~IRCClient();

        std::string const &                 get_nickname(void) const;
        std::string const &                 get_username(void) const;
        std::string const &                 get_realname(void) const;
        std::string const &                 get_mode(void) const;
        std::vector<std::string> const &    get_channels(void) const;

        void    set_nickname(std::string nickname);
        void    set_username(std::string username);
        void    set_realname(std::string realname);
        void    set_mode(std::string mode);
        void    set_channels(std::string channel);
        void    set_password(std::string password);

    private:

        int         _fd;

        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _mode;
        std::string _hostname;
        std::string _password;

        std::vector<std::string>    _channels;

        bool    _is_in_this_channel(std::string channel) const;

        IRCClient();
        IRCClient(IRCClient const & cpy);
        IRCClient & operator=(IRCClient const & rhs);

};

#endif