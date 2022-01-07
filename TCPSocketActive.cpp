#include "TCPSocketActive.hpp"

//DEBUG
#include <iostream>

/**
 *  @brief Constructs a TCPSocketActive given its accepted socketfd.
 */
TCPSocketActive::TCPSocketActive(int socketfd) {
	_socketfd = socketfd;
}

TCPSocketActive::~TCPSocketActive() {}

/**
 *  @brief Starts the socket by putting it in non-blocking mode.
 */
void TCPSocketActive::start() {
	fcntl(_socketfd, F_SETFL, O_NONBLOCK);
}

/**
 *  @brief Closes the socket's socketfd.
 */
void TCPSocketActive::close_fd() {
	shutdown(_socketfd, SHUT_RDWR);
	close(_socketfd);
}

/**
 *  @brief Receives TCP data from this socket and returns a string of it.
 *
 *  @return The received data as a string.
 */
std::string TCPSocketActive::receive_data() {
	char buf[BUFFER_SIZE];

	memset(buf, 0, BUFFER_SIZE);
	if (recv(_socketfd, &buf, BUFFER_SIZE - 1, 0) == -1) {
		if (errno != EWOULDBLOCK) {
			throw Cexception();
		}
	}
	std::string data(buf);

	//std::cout << "TCPSocketActive received : \"" << buf << "\"" << std::endl;

	return data;
}

/**
 *  @brief Sends TCP data to this socket.
 *
 *  @param data The data to be sent.
 */
void TCPSocketActive::send_data(std::string data) {
	if (send(_socketfd, data.c_str(), data.size(), 0) == -1) {
		if (errno != EWOULDBLOCK) {
			throw Cexception();
		} else {
			throw WouldBlockException();
		}
	}
}

/**
 *  @brief Getter for the socketfd
 *
 *  @return The socket's socketfd.
 */
int TCPSocketActive::get_socket_fd(void) const { return _socketfd; }