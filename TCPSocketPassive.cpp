#include "TCPSocketPassive.hpp"

/**
 *  @brief Construct a new TCPSocketPassive from a port.
 *
 *  @param port The TCPSocketPassive's bound port.
 */
TCPSocketPassive::TCPSocketPassive(int port) {
	int opt = 1;
	struct sockaddr_in6 serveraddr;

	if ((_socketfd = socket(AF_INET6, SOCK_STREAM, 0)) == -1)
		throw Cexception();
	if (setsockopt(_socketfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1)
		throw Cexception();
	fcntl(_socketfd, F_SETFL, O_NONBLOCK);

    memset(&serveraddr, 0, sizeof(serveraddr));
    serveraddr.sin6_family = AF_INET6;
	serveraddr.sin6_port   = htons(port);
	serveraddr.sin6_addr   = in6addr_any;
	
	if(bind(_socketfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) == -1)
        throw Cexception();
}

TCPSocketPassive::~TCPSocketPassive() {}

/**
 *  @brief Start the socket listening.
 */
void TCPSocketPassive::start() {
	if (listen(_socketfd, MAX_CONNECTIONS) == -1) {
		throw Cexception();
	}
}

/**
 *  @brief Closes the socket.
 */
void TCPSocketPassive::close_fd() {
	close(_socketfd);
}

/**
 *  @brief Accepts a new connection and returns the new socketfd.
 *
 *  @return The new socketfd.
 */
int TCPSocketPassive::accept_connection() {
	int new_socketfd;
	int addrlen = sizeof(_address);

	new_socketfd = accept(_socketfd, (struct sockaddr *) &_address, (socklen_t *) &addrlen);
	return new_socketfd;
}

/**
 *  @brief Getter for the socketfd
 *
 *  @return The socket's socketfd.
 */
int TCPSocketPassive::get_socket_fd(void) const { return _socketfd; }