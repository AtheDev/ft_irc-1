#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <poll.h>
#include <vector>


#include "TCPSocket.hpp"
#include "TCPSocketPassive.hpp"
#include "TCPClient.hpp"


void handler_signal(int num);

class TCPServer {
	public:

		TCPServer(const std::string & port);
		~TCPServer();

		void start();
		void stop();

		class ErrorSignalException : public std::exception {
			public:
				virtual const char * what() const throw();
		};

		class ErrorPollException : public std::exception {
			public:
				virtual const char * what() const throw();
		};

	private:

		TCPSocketPassive _socket;
		std::vector<struct pollfd> _pollfds;
		std::map<int, TCPClient *> _clients;

		void _run();
		void _add_clients();
		void _add_client(int socket_fd);
		void _remove_client(int socket_fd);
		void _send_to_all(std::string str);
		void _handle_reception(std::vector<struct pollfd>::iterator & it);

		TCPServer();
		TCPServer(TCPServer const & cpy);
		TCPServer & operator=(TCPServer const & rhs);
};

#endif
