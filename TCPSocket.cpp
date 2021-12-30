#include "TCPSocket.hpp"
#include "TCPSocketActive.hpp"
#include "TCPSocketPassive.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CONNECTIONS 42
#define BUFFER_SIZE 64

//FORBIDDEN EXTERNAL FUNCTIONS
#include <string.h>
#include <errno.h>
//FORBIDDEN EXTERNAL FUNCTIONS

/*
**	This is to handle c functions that edit errno global variable
*/

const char*
	TCPSocket::Cexception::what() const throw() { return strerror(errno); }

TCPSocketPassive::TCPSocketPassive(int port)
{
	int opt = 1;

	//TODO: How to handle both ipv4 and ipv6?
	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Cexception();
	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw Cexception();
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
    fcntl(_socket, F_SETFL, O_NONBLOCK);
	if (bind(_socket, (struct sockaddr *)&_address, sizeof(_address)) == -1)
		throw Cexception();
}

TCPSocketPassive::~TCPSocketPassive() {}

void
	TCPSocketPassive::start()
{
	if (listen(_socket, MAX_CONNECTIONS) == -1)
		throw Cexception();
}

void
	TCPSocketPassive::close_fd()
{
	//Close socket how?
    close(_socket);
}

int
	TCPSocketPassive::accept_connection()
{
	int new_socketfd;
    int addrlen = sizeof(_address);

	/*if ((*/new_socketfd = accept(_socket, (struct sockaddr *)&_address, (socklen_t *)&addrlen);
		//throw Cexception();
	return new_socketfd;
}

int     TCPSocketPassive::get_socket_fd(void) const {return _socket;}

TCPSocketActive::TCPSocketActive(int socketfd)
{
_socket = socketfd;
	/*int addrlen = sizeof(_address);
	struct sockaddr_in tmp;

	if ((_socket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Cexception();
	if (getsockname(socketfd, (struct sockaddr *)&tmp, (socklen_t *)&addrlen) == -1)
		throw Cexception();
	_address.sin_family = tmp.sin_family;
	_address.sin_port = tmp.sin_port;
	_address.sin_addr = tmp.sin_addr; //Is this how to get the server socket address?*/
}

TCPSocketActive::~TCPSocketActive() {}

void
	TCPSocketActive::start()
{
	fcntl(_socket, F_SETFL, O_NONBLOCK); //Does this need to be after connect?
	//if (connect(_socket, (struct sockaddr *)&_address, sizeof(_address)) == -1) //Does this need to be in constructor?
		//throw Cexception();
}

void    TCPSocketActive::set_socket_fd(int fd) {_socket = fd;}

void
	TCPSocketActive::close_fd()
{
	//Close socket how?
    close(_socket);
}

std::string
	TCPSocketActive::receive()
{
	char buf[BUFFER_SIZE];

	if (recv(_socket, &buf, BUFFER_SIZE, 0) == -1)
		throw Cexception();
	std::string r (buf);
	return r;
}

void
	TCPSocketActive::send_data(std::string s)
{
	if (send(_socket, s.c_str(), s.size(), 0) == -1)
		throw Cexception();
}

int     TCPSocketActive::get_socket_fd(void) const {return _socket;}