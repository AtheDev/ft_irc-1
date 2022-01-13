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
	IRCMessage message1 (":A B C :D E F");
	//std::cout << message1 << std::endl;

	std::cout << std::boolalpha;
	//std::cout << "expected: false\t" << fmatch("a", "ab") << std::endl;
	//std::cout << "expected: false\t" << fmatch("ab", "a") << std::endl;
	//std::cout << "expected: true\t" << fmatch("a", "a") << std::endl;
	//std::cout << "expected: true\t" << fmatch("ab", "ab") << std::endl;
	//std::cout << "expected: true\t" << fmatch("123", "%(-)[0-9]") << std::endl;
	//std::cout << "expected: true\t" << fmatch("127.0.0.1", "%(1-3)[0-9].%(1-3)[0-9].%(1-3)[0-9].%(1-3)[0-9]") << std::endl;
	//std::cout << "expected: true\t" << fmatch("abc123", "%(-)[a-z]+[0-9]") << std::endl;
	//std::cout << "expected: true\t" << fmatch("abc", "%(-)[%97%-%122%]") << std::endl;
	//std::cout << "expected: false\t" << fmatch("abc3", "%(-)[%97%-%122%]") << std::endl;
	//std::cout << "expected: true\t" << fmatch("abc123", "%(-)[%97%-%122%]+[%48%-%57%]") << std::endl;
	//std::cout << "expected: true\t" << fmatch("a", "a") << std::endl;
	//std::cout << "expected: true\t" << fmatch("ab", "%97%%98%") << std::endl;
	//std::cout << "expected: true\t" << fmatch("aaaa", "%(-4)%97%") << std::endl;
	//std::cout << "expected: true\t" << fmatch("aaaa", "%R(1-4)a%R") << std::endl;
	//std::cout << "expected: false\t" << fmatch("abaa", "%R(1-4)a%R") << std::endl;
	//std::cout << "expected: true\t" << fmatch("abab", "%R(1-4)ab%R") << std::endl;
	//std::cout << "expected: true\t" << fmatch("X.a.b.a.b", "X%R(1-4).%(1-1)[a-z]%R") << std::endl;
	std::cout << "expected: true\t" << fmatch("127.0.0.1", "%(1-3)[0-9]%R(1-3).%(1-3)[0-9]%R") << std::endl;

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