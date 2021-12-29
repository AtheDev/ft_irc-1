#include "TCPSocket.hpp"

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
	if ((_socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Cexception();
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw Cexception();
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(port);
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
	TCPSocketPassive::close()
{
	//Close socket how?
}

int
	TCPSocketPassive::accept_connection()
{
	int new_socketfd, addrlen = sizeof(_address);

	if ((new_socketfd = accept(_socketfd, (struct sockaddr *)&_address, (socklen_t *)&addrlen)) == -1)
		throw Cexception();
	return new_socketfd;
}

TCPSocketActive::TCPSocketActive(int socketfd)
{
	int addrlen = sizeof(_address);
	struct sockaddr_in tmp;

	if ((_socketfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw Cexception();
	if (getsockname(socketfd, (struct sockaddr *)&tmp, (socklen_t *)&addrlen) == -1)
		throw Cexception();
	_address.sin_family = tmp.sin_family;
	_address.sin_port = tmp.sin_port;
	_address.sin_addr = tmp.sin_addr; //Is this how to get the server socket address?
}

void
	TCPSocketActive::start()
{
	fcntl(_socketfd, F_SETFL, O_NONBLOCK); //Does this need to be after connect?
	if (connect(_socketfd, (struct sockaddr *)&_address, sizeof(_address)) == -1) //Does this need to be in constructor?
		throw Cexception();
}

void
	TCPSocketActive::close()
{
	//Close socket how?
}

std::string
	TCPSocketActive::receive_data()
{
	char buf[BUFFER_SIZE];

	if (recv(_socketfd, &buf, BUFFER_SIZE, 0) == -1)
		throw Cexception();
	std::string r (buf);
	return r;
}

void
	TCPSocketActive::send_data(std::string s)
{
	if (send(_socketfd, s.c_str(), s.size(), 0) == -1)
		throw Cexception();
}