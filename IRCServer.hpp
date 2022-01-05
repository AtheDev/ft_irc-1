#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP


#include "TCPServer.hpp"


class IRCServer {
	public:
		IRCServer(std::string port);
		virtual ~IRCServer();

		void start();
		void stop();

	private:
		TCPServer _tcp_server;

		void _run();
};


#endif
