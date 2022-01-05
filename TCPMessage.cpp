#include "TCPMessage.hpp"

TCPMessage::TCPMessage(int sender, std::string & payload) {
	_sender = sender;
	_receivers = std::vector<int>();
	_payload = payload;
}

TCPMessage::TCPMessage(std::vector<int> & receivers, std::string & payload) {
	_sender = 0;
	_receivers = receivers;
	_payload = payload;
}

TCPMessage::TCPMessage(int sender, std::vector<int> & receivers, std::string & payload) {
	_sender = sender;
	_receivers = receivers;
	_payload = payload;
}

TCPMessage::TCPMessage(const TCPMessage & other) {
	if (this == &other) {
		return ;
	}
	_sender = other._sender;
	_receivers = other._receivers;
	_payload = other._payload;
}


int TCPMessage::get_sender() const {
	return _sender;
}

const std::vector<int> & TCPMessage::get_receivers() const {
	return _receivers;
}

const std::string & TCPMessage::get_payload() const {
	return _payload;
}

std::ostream & operator<<(std::ostream & os, const TCPMessage & message) {
	os << "TCPMessage [sender: " << message._sender << "] ";
	os << "[receivers: ";
	std::vector<int> receivers = message.get_receivers();
	std::vector<int>::iterator it_receiver = receivers.begin();
	for (; it_receiver != message._receivers.end(); it_receiver++) {
		os << *it_receiver;
	}
	os << "] ";
	os << "\"" << message._payload << "\"";
	return os;
}