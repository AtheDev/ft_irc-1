#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP


#include "TCPServer.hpp"
#include "IRCMessage.hpp"

#define PASS "PASS"
#define NICK "NICK"

class IRCServer {
	public:
		IRCServer(std::string port);
		virtual ~IRCServer();

		void start();
		void stop();

	private:
		TCPServer _tcp_server;
		std::map<std::string, void(IRCServer::*)(IRCMessage & message)>	_commands;
		std::map<int, IRCClient *>	_clients;

		void _run();
		void	_add_clients(std::vector<int> & new_clients);

		void	_execute_command(IRCMessage & message);
		void	_execute_cap(IRCMessage & message);
		void	_execute_pass(IRCMessage & message);
		void	_execute_nick(IRCMessage & message);
		void	_execute_user(IRCMessage & message);
		void	_execute_join(IRCMessage & message);
		void	_execute_privmsg(IRCMessage & message);


};


#endif
