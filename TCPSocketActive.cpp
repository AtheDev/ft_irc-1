#include "TCPSocketActive.hpp"
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

TCPSocketActive::TCPSocketActive(int socketfd)
{
    _socketfd = socketfd;
}

TCPSocketActive::~TCPSocketActive() {}

void
	TCPSocketActive::start()
{
	fcntl(_socketfd, F_SETFL, O_NONBLOCK); //Does this need to be after connect?
}

void
	TCPSocketActive::close_fd()
{
	//Close socket how?
    close(_socketfd);
}

std::string
	TCPSocketActive::receive_data()
{
	char buf[BUFFER_SIZE];

    memset(buf, 0, BUFFER_SIZE);
	if (recv(_socketfd, &buf, BUFFER_SIZE, 0) == -1)
		throw Cexception();
	std::string r (buf);
	return r;
}

void
	TCPSocketActive::send_data(std::string s)
{
	s += "\n";
	if (send(_socketfd, s.c_str(), s.size(), 0) == -1)
		throw Cexception();
}

int     TCPSocketActive::get_socket_fd(void) const {return _socketfd;}