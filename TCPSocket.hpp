#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX_CONNECTIONS 42
#define BUFFER_SIZE 65536


class TCPSocket {
	public:

		virtual void start() = 0;
		virtual void close_fd() = 0;
		virtual int get_socket_fd() const = 0;

		class Cexception : public std::exception {
			public:
				virtual const char * what() const throw();
		};

		class WouldBlockException : public std::exception {
			public:
				virtual const char * what() const throw();
		};

	protected:

		int _socketfd;
		struct sockaddr_in _address;

	private:


};

#endif