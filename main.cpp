#include "TCPServer.hpp"
#include "TCPSocketPassive.hpp"
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
    TCPServer server(static_cast<std::string>(av[1]));
    try {

        server.start();
    }
    catch (TCPSocketPassive::ErrorGetAddrInfoException & e) {
        std::cout << e.what() << std::endl;
    }
    catch (TCPSocketPassive::ErrorSocketException & e) {
        std::cout << e.what() << std::endl;
    }
    catch (TCPSocketPassive::ErrorBindException & e) {
        std::cout << e.what() << std::endl;
    }
    catch (TCPSocketPassive::ErrorListenException & e) {
        std::cout << e.what() << std::endl;
    }
    catch (TCPServer::ErrorSignalException & e) {
        server.stop();
        std::cout << e.what() << std::endl;
    }
    catch (TCPSocketPassive::ErrorSetSockOptException & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}