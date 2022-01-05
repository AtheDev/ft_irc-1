#include "TCPServer.hpp"


/**
 *  @brief Builds the TCPServer and initializes its socket.
 *
 *  @param port Port number on which your server will accept incoming connections.
 */
TCPServer::TCPServer(const std::string& port) : _socket(atoi(port.c_str())) {}

/**
 *  @brief Destroys the TCPServer.
 */
TCPServer::~TCPServer() {}

/**
 *  @brief Initialize the pollfd structure of the TCPServer and launch the server.
 */
void TCPServer::start(bool run) {

	signal(SIGINT, handler_signal);
	signal(SIGQUIT, handler_signal);

	struct pollfd new_pollfd;

	new_pollfd.fd = _socket.get_socket_fd();
	new_pollfd.events = POLLIN;
	new_pollfd.revents = 0;
	_pollfds.push_back(new_pollfd);

	std::cout << "\033[0;34m" << "Server opened" << "\033[0m" << std::endl;
	_socket.start();
	if (run) {
		_run();
	}
}

/**
 *  @brief Closes all open sockets and frees the memory allocated for TCPClient.
 */
void TCPServer::stop(void) {

	_pollfds.clear();
	if (!(_clients.empty())) {
		std::map<int, TCPClient *>::iterator it = _clients.begin();
		for (; it != _clients.end(); it++) {
			it->second->get_socket().close_fd();
			delete _clients[it->first];
		}
		_clients.clear();
	}
	_socket.close_fd();
	std::cout << ("\n\033[0;34mServer stopped\033[0m") << std::endl;
}

/**
 *  @brief Function that starts an infinite loop to keep the server open
 *  until it receives a closing signal.
 *  Allows to add new TCPClient, to delete a TCPClient when it disconnects
 *	and to resend incoming messages.
 */
void TCPServer::_run(void) {

	while (true) {
		if (poll(&(*_pollfds.begin()), _pollfds.size(), -1) == -1) {
			throw ErrorPollException();
		}
		std::vector<struct pollfd>::iterator it = _pollfds.begin();
		std::vector<struct pollfd>::iterator ite = _pollfds.end();
		for (; it != ite; it++) {
			if (it->revents == POLLIN) {
				if (it->fd == _socket.get_socket_fd()) {
					_add_clients();
				} else {
					_handle_reception(it);
					if ((it + 1) == ite) {
						break;
					}
				}
			} else if (it->revents == POLLHUP && it->fd != _socket.get_socket_fd()) {
				std::cout << "Client n°" << it->fd << " disconnected." << std::endl;
			}
		}
		it = _pollfds.begin();
		for (; it != _pollfds.end(); it++) {
			it->revents = 0;
		}
	}
}


void TCPServer::update() {
	new_clients.clear();
	messages_received.clear();
	_send_messages();
	messages_to_be_sent.clear();

	if (poll(&(*_pollfds.begin()), _pollfds.size(), -1) == -1) {
		throw ErrorPollException();
	}

	std::vector<struct pollfd>::iterator it = _pollfds.begin();
	std::vector<struct pollfd>::iterator ite = _pollfds.end();
	for (; it != ite; it++) {
		if (it->revents == POLLHUP && it->fd != _socket.get_socket_fd()) {
			//TODO: This never seems to happen. Why ?
			_remove_client(it->fd);
		}
		if (it->revents == POLLIN) {
			if (it->fd == _socket.get_socket_fd()) {
				_add_clients();
			} else {
				_handle_reception(it);
			}
		}
	}
}

/**
 *  @brief Loop to accept all incoming connections and create new TCPClients.
 *
 *  @details
 *  Also adds the new clients to new_clients for connection management outside the class.
 */
void TCPServer::_add_clients(void) {
	int new_fd;

	while (true) {
		new_fd = _socket.accept_connection();
		if (new_fd < 0) {
			break;
		}
		_add_client(new_fd);
	}
}

/**
 *  @brief Creates a new TCPClient and insert it in the map _clients.
 *  Creates and adds the corresponding pollfd structure.
 *
 *  @param socket_fd Corresponds to the fd of the new socket newly created
 *	by the connection acceptance.
 */
void TCPServer::_add_client(int socket_fd) {
	TCPClient * new_client = new TCPClient(socket_fd);
	_clients.insert(std::pair<int, TCPClient *>(socket_fd, new_client));

	struct pollfd new_pollfd;

	new_pollfd.fd = socket_fd;
	new_pollfd.events = POLLIN | POLLHUP;
	new_pollfd.revents = 0;
	_pollfds.push_back(new_pollfd);
	new_clients.push_back(socket_fd);
	std::cout << "\033[0;32m" << "Client n°" << socket_fd << " connected." << "\033[0m" << std::endl;
}

/**
 *  @brief Destroy the TCPClient corresponding to the socket_fd following its disconnection.
 *
 *  @param socket_fd Corresponds to the fd of the TCPClient socket that disconnects
 */
void TCPServer::_remove_client(int socket_fd) {
	std::vector<struct pollfd>::iterator it_pollfd = _pollfds.begin();
	for (; it_pollfd != _pollfds.end(); it_pollfd++) {
		if (it_pollfd->fd == socket_fd) {
			_pollfds.erase(it_pollfd);
			break;
		}
	}
	_clients[socket_fd]->get_socket().close_fd();
	delete _clients[socket_fd];
	_clients.erase(socket_fd);
	std::cout << "\033[0;32m" << "Client n°" << socket_fd << " disconnected." << "\033[0m" << std::endl;
}

/**
 *  @brief Handles a message reception.
 *
 *  @param socket_fd Corresponds to the fd of the TCPClient socket that disconnects
 */
void TCPServer::_handle_reception(std::vector<struct pollfd>::iterator & it) {
	std::map<int, TCPClient *>::iterator it_client = _clients.find(it->fd);
	std::list<std::string> messages;
	try {
		messages = it_client->second->receive_from();
	} catch (TCPClient::DisconnectionException & e) {
		_remove_client(it->fd);
		return;
	}
	std::list<std::string>::iterator it_message = messages.begin();
	for (; it_message != messages.end(); it_message++) {
		TCPMessage new_message = TCPMessage(it->fd, *it_message);
		messages_received.push_back(new_message);
		std::cout << "Message received: " << new_message << std::endl;
	}
}

/**
 *  @brief Sends all messages stored in messages_to_be_sent then clears them
 */
void TCPServer::_send_messages() {
	std::list<TCPMessage>::iterator it_message = messages_to_be_sent.begin();
	for (; it_message != messages_to_be_sent.end(); it_message++) {
		_send_message(*it_message);
	}
}

/**
 *  @brief Sends a single message for each of its receivers.
 *
 *  @param message The message to be sent.
 */
void TCPServer::_send_message(TCPMessage & message) {
	std::vector<int> receivers = message.get_receivers();
	std::vector<int>::iterator it_receiver = receivers.begin();
	for (; it_receiver != receivers.end(); it_receiver++) {
		_clients[*it_receiver]->send_to(message.get_payload());
	}
}

const char * TCPServer::ErrorSignalException::what() const throw() {
	return ("\n\033[0;34mServer closed\033[0m");
}

const char * TCPServer::ErrorPollException::what() const throw() {
	return ("Exception : error poll()");
}
