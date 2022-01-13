#include "Channel.hpp"


Channel::Channel(std::string & name): _name(name) {}

Channel::~Channel() {}

const std::string & Channel::get_name() const {
	return _name;
}

void Channel::set_name(const std::string & name) {
	_name = name;
}

/**
 * @brief Tries to add a client to a channel.
 * @param socketfd The client socketfd.
 * @return true if adding was successful, else false.
 */
bool Channel::add_client(int socketfd) {
	if (std::find(_clients.begin(), _clients.end(), socketfd) == _clients.end()) {
		_clients.push_back(socketfd);
		return true;
	}
	return false;
}

/**
 * @brief Tries to remove a client from a channel.
 * @param socketfd The client socketfd.
 * @return true if removing was successful, else false.
 */
bool Channel::remove_client(int socketfd) {
	std::vector<int>::iterator it;
	if ((it = std::find(_clients.begin(), _clients.end(), socketfd)) != _clients.end()) {
		_clients.erase(it);
		return true;
	}
	return false;
}

const std::vector<int> & Channel::get_clients() const {
	return _clients;
}

/**
 * @brief Handle to iterate on client's socketfds of this channel.
 * @return A begin iterator to the clients.
 */
std::vector<int>::const_iterator Channel::clients_begin() const {
	return _clients.begin();
}

/**
 * @brief Handle to iterate on client's socketfds of this channel.
 * @return An end iterator to the clients.
 */
std::vector<int>::const_iterator Channel::clients_end() const {
	return _clients.end();
}

std::ostream & operator<<(std::ostream & os, const Channel & channel) {
	os << "Channel " << channel._name << ". Clients: ";
	std::vector<int>::const_iterator it_client = channel.clients_begin();
	for (; it_client != channel.clients_end(); it_client++) {
		os << *it_client << ", ";
	}
	return os;
}

/**
 * @brief Handle to iterate on socketfds of clients that are channel operators.
 * @return A begin iterator to channel operator clients.
 */
std::vector<int>::iterator Channel::channel_op_begin() {
	return _clients.begin();
}

/**
 * @brief Handle to iterate on socketfds of clients that are channel operators.
 * @return An end iterator to channel operator clients.
 */
std::vector<int>::iterator Channel::channel_op_end() {
	return _clients.end();
}

const std::string & Channel::get_topic() const {
	return _topic;
}

void Channel::set_topic(const std::string & topic) {
	_topic = topic;
}

std::vector<int> Channel::get_channel_op() const {
	return _channel_operators;
}