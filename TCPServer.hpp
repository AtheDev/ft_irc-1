#ifndef TCPSERVER_HPP
#define TCPSERVER_HPP

#include <algorithm>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <list>
#include <poll.h>
#include <queue>
#include <vector>


#include "TCPClient.hpp"
#include "TCPSocket.hpp"
#include "TCPSocketPassive.hpp"
#include "TCPMessage.hpp"


void handler_signal(int num);

class TCPServer {
	public:

		TCPServer(const std::string & port);
		~TCPServer();

		void start();
		void update();
		void stop();
		std::vector<int> & get_failed_clients_connections();
		std::vector<int> const & get_new_clients() const;
		std::vector<int> const & get_disconnected_clients() const;
		std::list<TCPMessage> const & get_messages_received() const;
		void schedule_sent_message(TCPMessage const & message);

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
		std::list<TCPMessage> _messages_failed_to_be_sent;
		std::vector<int> _new_clients;
		std::vector<int> _disconnected_clients;
		std::list<TCPMessage> _messages_received;
		std::list<TCPMessage> _messages_to_be_sent;
		std::vector<int> _failed_clients_connections;
		void _add_clients();
		void _add_client(int socket_fd);
		void _remove_client(int socket_fd);
		void _send_messages();
		void _send_message(TCPMessage & message);
		void _send_failed_messages();
		void _handle_reception(std::vector<struct pollfd>::iterator & it);
		void _add_failed_message(int receiver, std::string payload);

		TCPServer();
		TCPServer(TCPServer const & cpy);
		TCPServer & operator=(TCPServer const & rhs);
};

#endif
