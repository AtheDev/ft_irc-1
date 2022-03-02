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
void TCPServer::start() {

	signal(SIGINT, handler_signal);
	signal(SIGQUIT, handler_signal);

	struct pollfd new_pollfd;

	new_pollfd.fd = _socket.get_socket_fd();
	new_pollfd.events = POLLIN;
	new_pollfd.revents = 0;
	_pollfds.push_back(new_pollfd);

	std::cout << "\033[0;34m" << "Server opened" << "\033[0m" << std::endl;
	_socket.start();
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
 *  @brief Updates the server once.
 *
 *  @details
 *  1. Sends the messages stored in messages_to_be_sent\n
 *  2. Polls all socketfds.\n
 *  3. If messages are received, they are stored in messages_received.\n
 *  4. If new clients connected, they are stored in new_clients.\n
 *  5. If clients disconnected, they are stored in disconnected_clients.\n
 *  Note : messages_received, new_clients and disconnected_clients are cleared when update is called
 *  and messages_to_be_sent is cleared after the messages were all sent.
 */
void TCPServer::update() {
	_new_clients.clear();
	_disconnected_clients.clear();
	_messages_received.clear();
	_send_messages();
	_messages_to_be_sent.clear();

	std::vector<int>::iterator clients = _clients_to_disconnect.begin();
	for (; clients != _clients_to_disconnect.end(); clients++)
		_remove_client(*clients);
	_clients_to_disconnect.clear();

	if (poll(&(*_pollfds.begin()), _pollfds.size(), 100) == -1) {
		throw ErrorPollException();
	}

	std::vector<struct pollfd>::iterator it = _pollfds.begin();
	std::vector<struct pollfd>::iterator ite = _pollfds.end();
	for (; it != ite; it++) {
		if (it->revents & POLLHUP && it->fd != _socket.get_socket_fd()) {
			if (_clients.find(it->fd) != _clients.end()) {
				// Remove client only if it hasn't been removed first
				_remove_client(it->fd);
			}
		} else if (it->revents & POLLIN) {
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
	_new_clients.push_back(socket_fd);
	std::cout << "\033[0;32m" << "Client n°" << socket_fd << " connected." << "\033[0m" << std::endl;
}

/**
 *  @brief Destroy the TCPClient corresponding to the socket_fd following its disconnection.
 *
 *  @param socket_fd Corresponds to the fd of the TCPClient socket that disconnects
 */
void TCPServer::_remove_client(int socket_fd) {
	_clients[socket_fd]->get_socket().close_fd();
	std::vector<struct pollfd>::iterator it_pollfd = _pollfds.begin();
	for (; it_pollfd != _pollfds.end(); it_pollfd++) {
		if (it_pollfd->fd == socket_fd) {
			_pollfds.erase(it_pollfd);
			break;
		}
	}
	delete _clients[socket_fd];
	_clients.erase(socket_fd);
	_disconnected_clients.push_back(socket_fd);
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
		_messages_received.push_back(new_message);
	}
}

/**
 *  @brief Sends all messages stored in messages_to_be_sent then clears them
 */
void TCPServer::_send_messages() {
	_send_failed_messages();
	std::list<TCPMessage>::iterator it_message = _messages_to_be_sent.begin();
	for (; it_message != _messages_to_be_sent.end(); it_message++) {
		_send_message(*it_message);
	}
}

/**
 *  @brief Resend failed messages. If the message can't still be sent, save it again.
 *
 *  @param message The message to be sent.
 */
void TCPServer::_send_failed_messages() {
	std::list<TCPMessage>::iterator it_failed_message = _messages_failed_to_be_sent.begin();
	std::list<TCPMessage> tmp;
	for (; it_failed_message != _messages_failed_to_be_sent.end(); it_failed_message++) {
		// Failed messages are guaranteed to have a single receiver (the one that failed)
		int receiver = it_failed_message->get_receivers()[0];
		try {
			_clients.at(receiver)->send_to(it_failed_message->get_payload());
		} catch (TCPSocket::Cexception & e) {
			// If an error happened during the sending, the client failed we need to remove it
			_remove_client(receiver);
		} catch (std::out_of_range & e) {
			// Ignore if the client doesn't exist anymore !
		} catch (TCPSocket::WouldBlockException & e) {
			// If EWOULDBLOCK is still received, store the message in tmp
			tmp.push_back(*it_failed_message);
		}
	}
	// Clear failed messages
	_messages_failed_to_be_sent.clear();
	// Copy messages that failed to be sent again
	_messages_failed_to_be_sent = tmp;
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
		try {
			_clients.at(*it_receiver)->send_to(message.get_payload());
		} catch (TCPSocket::Cexception & e) {
			// If an error happened during the sending, the client failed we need to remove it
			_remove_client(*it_receiver);
		} catch (std::out_of_range & e) {
			// Ignore if the client doesn't exist anymore !
		} catch (TCPSocket::WouldBlockException & e) {
			_add_failed_message(*it_receiver, message.get_payload());
		}
	}
}

/**
 *  @brief Adds a message to the list of messages that failed to be sent.
 *
 *  @param receiver The receiver socketfd.
 *  @param payload A copy of the message payload.
 */
void TCPServer::_add_failed_message(int receiver, std::string payload) {
	std::vector<int> receivers(1, receiver);
	TCPMessage failed_message(receivers, payload);
	_messages_failed_to_be_sent.push_back(failed_message);
}

std::vector<int> const & TCPServer::get_new_clients() const {
	return _new_clients;
}

std::vector<int> const & TCPServer::get_disconnected_clients() const {
	return _disconnected_clients;
}

std::list<TCPMessage> const & TCPServer::get_messages_received() const {
	return _messages_received;
}

void TCPServer::schedule_sent_message(const TCPMessage & message) {
	_messages_to_be_sent.push_back(message);
}

void TCPServer::add_client_to_disconnect(int socketfd) {
	_clients_to_disconnect.push_back(socketfd);
}

const char * TCPServer::ErrorSignalException::what() const throw() {
	return ("\n\033[0;34mServer closed\033[0m");
}

const char * TCPServer::ErrorPollException::what() const throw() {
	return ("Exception : error poll()");
}
