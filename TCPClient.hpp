#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <list>
#include <string>
#include "TCPSocketActive.hpp"
#include "TCPSocket.hpp"


class TCPClient {

	public:

		TCPClient(int socketfd);
		virtual ~TCPClient();

		std::list<std::string> receive_from();
		void send_to(std::string message);
		TCPSocketActive & get_socket();

		class DisconnectionException : public std::exception {
			public:
				virtual const char * what() const throw();
		};

	private:

		std::string _buffer;
		TCPSocketActive _socket;
};


#endif
