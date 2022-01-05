#include "IRCServer.hpp"


IRCServer::IRCServer(std::string port): _tcp_server(port) {}

IRCServer::~IRCServer() {}

void IRCServer::start() {
	_tcp_server.start(false);
	_run();
}

void IRCServer::stop() {
	_tcp_server.stop();
}


void IRCServer::_run() {
	while (true) {
		// Préparer les messages à envoyer
		// Appel à update
		try {
			_tcp_server.update();
		} catch (TCPServer::ErrorSignalException & e) {
			throw e;
		}
		// Récupérer les nouveaux clients
		// Récupérer les nouveaux messages reçus.
	}
}


