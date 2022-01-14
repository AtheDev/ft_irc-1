//#include "IRCServer.hpp"
//#include "TCPServer.hpp"
//#include "TCPSocketPassive.hpp"
//#include "TCPSocketActive.hpp"
//#include "TCPSocket.hpp"
//#include <iostream>
//
//
//void	handler_signal(int num)
//{
//	(void)num;
//	throw TCPServer::ErrorSignalException();
//}

#include <iostream>
#include "IRCMessage.hpp"
#include "Message.hpp"

#include <cstdio>

bool fmatch(std::string token, std::string format);

int main(int ac, char **av)
{
	std::cout << std::boolalpha;
	fmatch("i", "%[%0x61:b]");

	//if (ac != 3)
	//{
	//	std::cout << "./ircserv <port> <password>" << std::endl;
	//	return 1;
	//}
	//IRCServer server(static_cast<std::string>(av[1]));
	//try {
	//	server.start();
	//}
	//catch (TCPServer::ErrorPollException & e) {
	//	std::cout << e.what() << std::endl;
	//}
	//catch (TCPServer::ErrorSignalException & e) {
	//	server.stop();
	//	std::cout << e.what() << std::endl;
	//}
	//catch (TCPSocket::Cexception & e) {
	//	std::cout << e.what() << std::endl;
	//}

	return 0;
}