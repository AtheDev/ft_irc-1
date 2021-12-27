#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <list>
#include <string>
#include "TCPSocketActive.hpp"

class TCPClient {
public:
	TCPClient(int socketfd);
	virtual ~TCPClient()

	std::list<std::string> receive_from();
	void send_to(std::string message);

private:
	std::string _buffer;
	TCPSocketActive _socket;
};


#endif
