#include "Channel.hpp"


Channel::Channel(std::string & name): _name(name) {}

Channel::~Channel() {}

const std::string & Channel::get_name() const {
	return _name;
}

void Channel::set_name(const std::string & name) {
	_name = name;
}

void Channel::add_client(int socketfd) {
	if (std::find(_clients.begin(), _clients.end(), socketfd) == _clients.end()) {
		_clients.push_back(socketfd);
	}
}

void Channel::remove_client(int socketfd) {
	std::vector<int>::iterator it;
	if ((it = std::find(_clients.begin(), _clients.end(), socketfd)) != _clients.end()) {
		_clients.erase(it);
	}
}

std::vector<int>::iterator Channel::clients_begin() {
	return _clients.begin();
}

std::vector<int>::iterator Channel::clients_end() {
	return _clients.end();
}
