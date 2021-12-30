#include "TCPClient.hpp"


/**
 *  @brief Builds a TCPClient and initializes and starts its socket.
 *
 *  @param socketfd  The socketfd of the socket's client
 */
TCPClient::TCPClient(int socketfd) : _socket(socketfd) {
	_socket.start();
}

/**
 *  @brief Closes the client's socket and destroys the client.
 */
TCPClient::~TCPClient() {
	_socket.close_fd();
}

/**
 *  @brief Receive data from the client.
 *
 *  @details
 *  Receives partial data from the client, appends it to the internal buffer and if CRLF is found,
 *  creates and returns a list of received messages.
 *
 *  @return A list of the messages received.
 */
std::list <std::string> TCPClient::receive_from() {
	std::string received_data = _socket.receive();
	_buffer += received_data;

	size_t pos;
	std::string substring;
	std::string delimiter = "\n"; //"\r\n"; // CRLF
	std::list <std::string> messages;
	while ((pos = _buffer.find(delimiter)) != std::string::npos) {
		substring = _buffer.substr(0, pos);
		messages.push_back(substring);
		_buffer.erase(0, pos + delimiter.length());
	}
	return messages;
}

/**
 *  @brief Sends a message to the client.
 *
 *  @param message  The message to be sent.
 */
void TCPClient::send_to(std::string message) {
	_socket.send_data(message);
}

TCPSocketActive     TCPClient::get_socket(void) const {return _socket;}