#include "TCPSocketPassive.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CONNECTIONS 42
#define BUFFER_SIZE 65535

//FORBIDDEN EXTERNAL FUNCTIONS
#include <string.h>
#include <errno.h>
//FORBIDDEN EXTERNAL FUNCTIONS

/**
 *  @brief Exception class for C function errors that return -1 and edit errno
 *
 *  @return The raised error's string
 */

const char*
	TCPSocket::Cexception::what() const throw() { return strerror(errno); }

TCPSocketPassive::TCPSocketPassive(int port)
{
	int opt = 1;

	//TODO: How to handle both ipv4 and ipv6?
	if ((_socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Cexception();
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw Cexception();
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
    fcntl(_socketfd, F_SETFL, O_NONBLOCK);
	if (bind(_socketfd, (struct sockaddr *)&_address, sizeof(_address)) == -1)
		throw Cexception();
}

TCPSocketPassive::~TCPSocketPassive() {}

void
	TCPSocketPassive::start()
{
	if (listen(_socketfd, MAX_CONNECTIONS) == -1)
		throw Cexception();
}

void
	TCPSocketPassive::close_fd()
{
	//Close socket how?
    close(_socketfd);
}

int
	TCPSocketPassive::accept_connection()
{
	int new_socketfd;
    int addrlen = sizeof(_address);

	new_socketfd = accept(_socketfd, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
	return new_socketfd;
}

int     TCPSocketPassive::get_socket_fd(void) const {return _socketfd;}