#include "TCPMessage.hpp"

/**
 *  @brief Constructs a TCPMessage on receiving.
 *
 *  @param sender  The socketfd of the client sending the message.
 *  @param payload  The message's payload.
 */
TCPMessage::TCPMessage(int sender, std::string & payload) {
	_sender = sender;
	_receivers = std::vector<int>(); // _receivers is empty
	_payload = payload;
}

/**
 *  @brief Constructs a TCPMessage on sending.
 *
 *  @param receivers  The socketfd of the clients receiving the message.
 *  @param payload  The message's payload.
 */
TCPMessage::TCPMessage(std::vector<int> & receivers, std::string & payload) {
	_sender = 0; // _sender is empty
	_receivers = receivers;
	_payload = payload;
}

/**
 *  @brief Constructs a TCPMessage with all attributes.
 *
 *  @param sender  The socketfd of the client sending the message.
 *  @param receivers  The socketfd of the clients receiving the message.
 *  @param payload  The message's payload.
 */
TCPMessage::TCPMessage(int sender, std::vector<int> & receivers, std::string & payload) {
	_sender = sender;
	_receivers = receivers;
	_payload = payload;
}

/**
 *  @brief Copy construct a TCPMessage.
 */
TCPMessage::TCPMessage(const TCPMessage & other) {
	if (this == &other) {
		return ;
	}
	_sender = other._sender;
	_receivers = other._receivers;
	_payload = other._payload;
}

/**
 *  @brief Gets the sender.
 */
int TCPMessage::get_sender() const {
	return _sender;
}

/**
 *  @brief Gets a reference on the receivers.
 */
const std::vector<int> & TCPMessage::get_receivers() const {
	return _receivers;
}

/**
 *  @brief Gets a reference on the payload.
 */
const std::string & TCPMessage::get_payload() const {
	return _payload;
}

/**
 *  @brief Stream operator overload for debug purposes.
 */
std::ostream & operator<<(std::ostream & os, const TCPMessage & message) {
	os << "TCPMessage [sender: " << message._sender << "] ";
	os << "[receivers: ";
	std::vector<int> receivers = message.get_receivers();
	std::vector<int>::iterator it_receiver = receivers.begin();
	for (; it_receiver != receivers.end(); it_receiver++) {
		os << *it_receiver;
	}
	os << "] ";
	os << "\"" << message._payload << "\"";
	return os;
}