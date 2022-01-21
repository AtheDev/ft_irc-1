#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <ctime>
#include <iostream>
#include <map>
#include <string>

#include "TCPServer.hpp"
#include "IRCMessage.hpp"
#include "IRCClient.hpp"
#include "Reply.hpp"
#include "Channel.hpp"
#include "Utils.hpp"

class IRCMessage;

class IRCServer {
	public:
		IRCServer(std::string port);
		virtual ~IRCServer();

		void start();
		void stop();

		std::map<int, IRCClient *>  get_clients(void) const;
		std::string const &			get_servername(void) const;
		std::string const &			get_version(void) const;
		std::string const &			get_server_creation_date() const;

		std::map<int, IRCClient *>::const_iterator			find_nickname(std::string & nickname) const;
		std::map<std::string, Channel *>::const_iterator	find_channel(std::string & channel_name) const;


	private:
		TCPServer _tcp_server;
		std::string	_servername;
		std::string	_version;
		std::string	_server_creation_date;
		std::map<std::string, void(IRCServer::*)(IRCMessage const & message)>	_commands;
		std::map<int, IRCClient *>	_clients;
		std::map<std::string, Channel *> _channels;


		void	_run();
		void	_add_clients(const std::vector<int> & new_clients);
		void	_remove_clients(const std::vector<int> & disconnected_clients);
		void	_remove_client_from_all_channels(int client_socketfd);

		void	_execute_command(IRCMessage const & message);
		void	_execute_pass(IRCMessage const & message);
		void	_execute_nick(IRCMessage const & message);
		void	_execute_user(IRCMessage const & message);
//		void	_execute_mode(IRCMessage const & message);
		void	_execute_quit(IRCMessage const & message);
		void	_execute_join(IRCMessage const & message);
		void	_execute_part(IRCMessage const & message);
		void	_execute_privmsg(IRCMessage const & message);
		void	_execute_notice(IRCMessage const & message);
		void	_execute_topic(IRCMessage const & message);
		void	_execute_names(IRCMessage const & message);
		void	_execute_list(IRCMessage const & message);
//		void	_execute_whois(IRCMessage const & message);
		void	_execute_ping(IRCMessage const & message);
		void	_execute_away(IRCMessage const & message);

		std::vector<Channel *> _get_client_channels(int client_socketfd);
		std::string _get_formatted_clients_from_channel(const std::string & channel_name);
		std::string _get_formatted_clients_without_channel(void);
};


#endif
