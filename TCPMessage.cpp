#include "TCPMessage.hpp"


TCPMessage::TCPMessage(const int sender, const std::vector<int>& receivers, std::string payload):
	_sender(sender), _receivers(receivers), _payload(payload)
{}

int TCPMessage::get_sender() const {
	return _sender;
}

std::vector<int> TCPMessage::get_receivers() const {
	return _receivers;
}

std::string TCPMessage::get_payload() const {
	return _payload;
}

std::ostream & operator<<(std::ostream & os, const TCPMessage & message) {
	os << "TCPMessage [sender: " << message._sender << "]";
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
