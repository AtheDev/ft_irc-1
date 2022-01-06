#include "IRCServer.hpp"

#include <iostream>
IRCServer::IRCServer(std::string port): _tcp_server(port) {
	_commands["PASS"] = &IRCServer::_execute_pass;
	_commands["NICK"] = &IRCServer::_execute_nick;
	_commands["USER"] = &IRCServer::_execute_user;
	_commands["JOIN"] = &IRCServer::_execute_join;
	_commands["PRIVMSG"] = &IRCServer::_execute_privmsg;
}

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
		_tcp_server.update();
		// Récupérer les nouveaux clients
		// Récupérer les nouveaux messages reçus.
		IRCMessage tmp("PASS :coucou");
		tmp.set_command("PASS");
std::cout << "tmp : " << tmp.get_command() << std::endl;

		_execute_command(tmp);
	}
}

void IRCServer::_execute_command(IRCMessage & message) {

	(this->*_commands[message.get_command()])(message);
}

void IRCServer::_execute_pass(IRCMessage & message) {
	std::cout << "commande pass : " << message.get_command() << std::endl;
}

void IRCServer::_execute_nick(IRCMessage & message) {
	std::cout << "commande nick: " << message.get_command() << std::endl;
}

void IRCServer::_execute_user(IRCMessage & message) {
	std::cout << "commande user : " << message.get_command() << std::endl;
}

void IRCServer::_execute_join(IRCMessage & message) {
	std::cout << "commande join: " << message.get_command() << std::endl;
}

void IRCServer::_execute_privmsg(IRCMessage & message) {
	std::cout << "commande privmsg: " << message.get_command() << std::endl;
}

/*NICK
USER
PASS
JOIN
PRIVMSG


Operator
	SQUIT
	CONNECT
	KILL


PASS en premier mais pas obligatoire (devant NICK/USER)
NICK/USER
	si success -> RPL_WELCOME doit contenir l'identifiant client complet 
*/