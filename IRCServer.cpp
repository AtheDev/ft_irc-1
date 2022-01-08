#include "IRCServer.hpp"

#include <iostream>
IRCServer::IRCServer(std::string port): _tcp_server(port), _servername("IRC Server VTA !"), _version("42.42") {

	_commands["PASS"] = &IRCServer::_execute_pass;
	_commands["NICK"] = &IRCServer::_execute_nick;
	_commands["USER"] = &IRCServer::_execute_user;
	_commands["QUIT"] = &IRCServer::_execute_quit;
	_commands["JOIN"] = &IRCServer::_execute_join;
	_commands["PRIVMSG"] = &IRCServer::_execute_privmsg;
}

IRCServer::~IRCServer() {}

void IRCServer::start() {
	_tcp_server.start();
	_run();
}

void IRCServer::stop() {
	_tcp_server.stop();
	_commands.clear();
	if (!(_clients.empty())) {
		std::map<int, IRCClient *>::iterator it = _clients.begin();
		for (; it != _clients.end(); it++)
			delete _clients[it->first];
		_clients.clear();
	}
}

std::map<int, IRCClient *>  IRCServer::get_clients(void) const { return _clients;}
std::string const &			IRCServer::get_servername(void) const { return _servername;}
std::string const &			IRCServer::get_version(void) const {return _version;}

void IRCServer::_run() {
	while (true) {
		// Préparer les messages à envoyer
		// Appel à update
		try {
			_tcp_server.update();
		} catch (TCPServer::ErrorSignalException & e) {
			throw e;
		}
		_add_clients(_tcp_server.new_clients);
		_remove_clients(_tcp_server.disconnected_clients);
		// Récupérer les nouveaux clients
		// Récupérer les nouveaux messages reçus.

		std::list<TCPMessage>::iterator it_message = _tcp_server.messages_received.begin();
		for (; it_message != _tcp_server.messages_received.end(); it_message++)
		{
			IRCMessage tmp(it_message->get_sender(), it_message->get_payload());
			_execute_command(tmp);
		}
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

void IRCServer::_remove_clients(std::vector<int> & disconnected_clients) {
	std::vector<int>::iterator it = disconnected_clients.begin();
	for (; it != disconnected_clients.end(); it++)
	{
		delete _clients[*it];
		_clients.erase(*it);
	}
}

void IRCServer::_execute_command(IRCMessage & message) {

	std::map<std::string, void(IRCServer::*)(IRCMessage & message)>::iterator it = _commands.find(message.get_command());
	if (it != _commands.end())
		(this->*_commands[message.get_command()])(message);
	else
		std::cout << "Command received: " << message.get_command() << std::endl;
}

void IRCServer::_execute_pass(IRCMessage & message) {
	std::cout << "commande pass : " << message.get_command() << std::endl;
	if (_clients[message.get_sender()]->get_status() == UNREGISTERED)
	{
		_clients[message.get_sender()]->set_password(message.get_params()[0]);
		_clients[message.get_sender()]->set_status(PASSWORD);
	}
	else
	{
		IRCMessage message_to_send(ERR_ALREADYREGISTRED, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send.to_tcp_message());
	}
}

void IRCServer::_execute_nick(IRCMessage & message) {
	std::cout << "commande nick: " << message.get_command() << std::endl;
	if (_clients[message.get_sender()]->get_nickname() == message.get_params()[0])
	{
		IRCMessage message_to_send(ERR_NICKNAMEINUSE, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send.to_tcp_message());
		return ;
	}
	else if (_check_nickname_in_use(message.get_params()[0]) == true)
	{
		IRCMessage message_to_send(ERR_NICKCOLLISION, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send.to_tcp_message());
		return ;
	}
	_clients[message.get_sender()]->set_nickname(message.get_params()[0]);
	if (_clients[message.get_sender()]->get_status() == PASSWORD
	|| _clients[message.get_sender()]->get_status() == UNREGISTERED)
		_clients[message.get_sender()]->set_status(NICKNAME);
}

void IRCServer::_execute_user(IRCMessage & message) {
	std::cout << "commande user : " << message.get_command() << std::endl;

	if (_clients[message.get_sender()]->get_status() == REGISTERED)
	{
		IRCMessage message_to_send(ERR_ALREADYREGISTRED, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send.to_tcp_message());
	}
	else if (_clients[message.get_sender()]->get_status() == NICKNAME)
	{
		_clients[message.get_sender()]->set_username(message.get_params()[0]);
		_clients[message.get_sender()]->set_hostname(message.get_params()[1]);
		//_clients[message.get_sender()]->set_servername(message.get_params()[2]);
		_clients[message.get_sender()]->set_realname(message.get_params()[3]);
		_clients[message.get_sender()]->set_status(REGISTERED);
		IRCMessage message_to_send1(RPL_WELCOME, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send1.to_tcp_message());
		IRCMessage message_to_send2(RPL_YOURHOST, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send2.to_tcp_message());
		IRCMessage message_to_send3(RPL_CREATED, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send3.to_tcp_message());
		IRCMessage message_to_send4(RPL_MYINFO, *this, message);
		_tcp_server.messages_to_be_sent.push_back(message_to_send4.to_tcp_message());
	}
}

void IRCServer::_execute_quit(IRCMessage & message) {
	std::cout << "commande quit: " << message.get_command() << std::endl;
	/*
		TODO: voir comment supprimer comme on recoit une liste de clients déconnectés
		est-ce qu'on renvoie une liste au TCPServer? des clients qu on fait la commande QUIT ?

		+ message en parametre à qui il est destiné ?
	*/
}

void IRCServer::_execute_join(IRCMessage & message) {
	std::cout << "commande join: " << message.get_command() << std::endl;
}

void IRCServer::_execute_privmsg(IRCMessage & message) {
	std::cout << "commande privmsg: " << message.get_command() << std::endl;
}

bool IRCServer::_check_nickname_in_use(std::string & nickname) const {
	std::map<int, IRCClient *>::const_iterator it = _clients.begin();
	for (; it != _clients.end(); it++)
		if (it->second->get_nickname() == nickname)
			return true;
	return false;
}

std::map<int, IRCClient *>::const_iterator IRCServer::find_nickname(std::string & nickname) const {
	std::map<int, IRCClient *>::const_iterator it = _clients.begin();
	for (; it != _clients.end(); it++)
		if (it->second->get_nickname() == nickname)
			break;
	return it;
}