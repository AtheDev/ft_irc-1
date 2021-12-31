#include "TCPSocketActive.hpp"

TCPSocketActive::TCPSocketActive(int socketfd) {
	_socketfd = socketfd;
}

TCPSocketActive::~TCPSocketActive() {}

void TCPSocketActive::start() {
	fcntl(_socketfd, F_SETFL, O_NONBLOCK);
}

void TCPSocketActive::close_fd() {
	close(_socketfd);
}

std::string TCPSocketActive::receive_data() {
	char buf[BUFFER_SIZE];

	memset(buf, 0, BUFFER_SIZE);
	if (recv(_socketfd, &buf, BUFFER_SIZE, 0) == -1) {
		throw Cexception();
	}
	std::string r(buf);
	return r;
}

void TCPSocketActive::send_data(std::string s) {
	s += "\n";
	if (send(_socketfd, s.c_str(), s.size(), 0) == -1) {
		throw Cexception();
	}
}

int TCPSocketActive::get_socket_fd(void) const { return _socketfd; }