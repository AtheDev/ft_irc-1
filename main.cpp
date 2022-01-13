#include "IRCServer.hpp"
#include "TCPServer.hpp"
#include <iostream>


void	handler_signal(int num)
{
	(void)num;
	throw TCPServer::ErrorSignalException();
}

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "./ircserv <port> <password>" << std::endl;
		return 1;
	}
	IRCServer server(static_cast<std::string>(av[1]));
	try {
		server.start();
	}
	catch (TCPServer::ErrorPollException & e) {
		std::cout << e.what() << std::endl;
	}
	catch (TCPServer::ErrorSignalException & e) {
		server.stop();
		std::cout << e.what() << std::endl;
	}
	catch (TCPSocket::Cexception & e) {
		std::cout << e.what() << std::endl;
	}

	return 0;
}