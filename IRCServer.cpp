#include "IRCServer.hpp"

#include <iostream>
IRCServer::IRCServer(std::string port): _tcp_server(port) {
	_commands["CAP"] = &IRCServer::_execute_cap;
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

void IRCServer::_add_clients(std::vector<int> & new_clients) {
	std::vector<int>::iterator it = new_clients.begin();
	for (; it != new_clients.end(); it++)
	{
		IRCClient * new_client = new IRCClient(*it);
		_clients.insert(std::pair<int, IRCClient *>(*it, new_client));
	}
}

void IRCServer::_execute_command(IRCMessage & message) {

	(this->*_commands[message.get_command()])(message);
}

void IRCServer::_execute_cap(IRCMessage & message) {
	std::cout << "commande cap : " << message.get_command() << std::endl;
	if (_tcp_server)
	// renvoie un reply => CAP * LS : ca aucune fonctionnalité de capacité disponible
}

void IRCServer::_execute_pass(IRCMessage & message) {
	std::cout << "commande pass : " << message.get_command() << std::endl;
	//OPTIONAL
	if déjà enregistrer
	// renvoie reply => ERR_ALREADYREGISTRED
	//if pas assez de parametres
	// renvoie reply => ERR_NEEDMOREPARAMS
	//if pas encore enregistrer
	// 'Client_en_question'.set_password(get_params()[0])
}

void IRCServer::_execute_nick(IRCMessage & message) {
	std::cout << "commande nick: " << message.get_command() << std::endl;
	//if pas de param
	// renvoie reply => ERR_NONICKNAMEGIVEN
	//if check déjà utiliser et que oui
	// renvoie reply => ERR_NICKNAMEINUSE
	//if pas encore enregistrer
	//	on enregistre le nickname
	//if enregistrer
	// modifie le nickname
}

void IRCServer::_execute_user(IRCMessage & message) {
	std::cout << "commande user : " << message.get_command() << std::endl;
	// réagi selon la RFC 1459
	// USER <username> <hostname> <servername> <realname>
	//if déjà enregistrer
	// renvoie reply => ERR_ALREADYREGISTRED
	//if pas assez de parametres
	// renvoie reply => ERR_NEEDMOREPARAMS
	//if pas encore enregistrer
	//	on enregistre le nickname et
	//  on met en place le renvoie de RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO
	//=> On passe le status en CONNECT
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



"essage received: TCPMessage [sender: 4][receivers: ] "CAP LS
"essage received: TCPMessage [sender: 4][receivers: ] "NICK user42
Message received: TCPMessage [sender: 4][receivers: ] "USER user42 user42 localhost :"ser42



*/