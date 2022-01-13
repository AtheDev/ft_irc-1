#include "IRCServer.hpp"

IRCServer::IRCServer(std::string port): _tcp_server(port), _servername("IRC Server VTA !"), _version("42.42") {

	time_t raw_time;
	time(&raw_time);
	_server_creation_date = ctime(&raw_time);
	_server_creation_date.erase(--_server_creation_date.end());

	_commands["PASS"] = &IRCServer::_execute_pass;
	_commands["NICK"] = &IRCServer::_execute_nick;
	_commands["USER"] = &IRCServer::_execute_user;
	_commands["QUIT"] = &IRCServer::_execute_quit;
	_commands["JOIN"] = &IRCServer::_execute_join;
	_commands["PRIVMSG"] = &IRCServer::_execute_privmsg;
	_commands["TOPIC"] = &IRCServer::_execute_topic;
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
std::string const &			IRCServer::get_server_creation_date(void) const {return _server_creation_date;}

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
			IRCMessage tmp(*it_message);
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
	std::vector<int>::iterator it_client = disconnected_clients.begin();
	for (; it_client != disconnected_clients.end(); it_client++)
	{
		_remove_client_from_channels(*it_client);
		delete _clients[*it_client];
		_clients.erase(*it_client);
	}
}

void IRCServer::_remove_client_from_channels(int client_socketfd) {
	std::map<std::string, Channel *>::iterator it_channel = _channels.begin();
	for (; it_channel != _channels.end(); it_channel++) {
		it_channel->second->remove_client(client_socketfd);
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
		std::vector<int> receivers;
		receivers.push_back(message.get_sender());
		std::string	reply = ERR_ALREADYREGISTRED();
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
	}
}

void IRCServer::_execute_nick(IRCMessage & message) {
	std::cout << "commande nick: " << message.get_command() << std::endl;
	std::map<int, IRCClient *>::const_iterator it;
	if (_clients[message.get_sender()]->get_nickname() == message.get_params()[0])
	{
		std::vector<int> receivers;
		receivers.push_back(message.get_sender());
		std::string	reply = ERR_NICKNAMEINUSE(message.get_params()[0]);
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
	}
	else if ((it = find_nickname(message.get_params()[0])) != _clients.end())
	{
		std::vector<int> receivers;
		receivers.push_back(message.get_sender());
		std::string	reply = ERR_NICKCOLLISION(message.get_params()[0],
								it->second->get_username(), it->second->get_hostname());
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
	}
	else
	{
		_clients[message.get_sender()]->set_nickname(message.get_params()[0]);
		if (_clients[message.get_sender()]->get_status() == PASSWORD
		|| _clients[message.get_sender()]->get_status() == UNREGISTERED)
			_clients[message.get_sender()]->set_status(NICKNAME);
	}
}

void IRCServer::_execute_user(IRCMessage & message) {
	std::cout << "commande user : " << message.get_command() << std::endl;

	if (_clients[message.get_sender()]->get_status() == REGISTERED)
	{
		std::vector<int> receivers;
		receivers.push_back(message.get_sender());
		std::string	reply = ERR_ALREADYREGISTRED();
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
	}
	else if (_clients[message.get_sender()]->get_status() == NICKNAME)
	{
		_clients[message.get_sender()]->set_username(message.get_params()[0]);
		_clients[message.get_sender()]->set_hostname(message.get_params()[1]);
		//_clients[message.get_sender()]->set_servername(message.get_params()[2]);
		_clients[message.get_sender()]->set_realname(message.get_params()[3]);
		_clients[message.get_sender()]->set_status(REGISTERED);

	std::string user_modes("USER_MODES"), channel_modes("CHANNEL_MODES");
		std::vector<int> receivers;
		receivers.push_back(message.get_sender());
		std::string	reply = RPL_WELCOME(get_clients()[message.get_sender()]->get_nickname(),
										get_clients()[message.get_sender()]->get_username(),
										get_clients()[message.get_sender()]->get_hostname());
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		reply = RPL_YOURHOST(get_servername(), get_version());
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		reply = RPL_CREATED(get_server_creation_date());
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		reply = RPL_MYINFO(get_servername(), get_version(), user_modes, channel_modes);
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
	}
}

void IRCServer::_execute_quit(IRCMessage & message) {
	std::cout << "commande quit: " << message.get_command() << std::endl;
	std::vector<int> receivers;
	std::string quit_message;
	if (message.get_params().empty())
		quit_message = "Has disconnected.";
	else
	{
		for (size_t i = 0; i < message.get_params().size(); i++)
			quit_message += (message.get_params()[i] + " ");
		quit_message.erase(--quit_message.end());
	}
	IRCClient * client = _clients[message.get_sender()];
	std::vector<std::string>::iterator it = client->get_channels().begin();
	/*for (chaque channel)
	{
		remove_client(message.get_sender());
		for ()
	//receivers.push_back(message.get_sender());
	//std::string	reply = ERR_ALREADYREGISTRED();
	_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
	}*/
}

/**
 * @brief Executes a JOIN command.
 * @param message The message containing the JOIN command.
 */
void IRCServer::_execute_join(IRCMessage & message) {
	std::cout << "commande join: " << message.get_command() << std::endl;
	//TODO: For now, it doesn't use keys and can only manage a single channel.
	std::string channel_name = message.get_params()[0];
	try {
		// If channel exists, add_client to channel.
		_channels.at(channel_name)->add_client(message.get_sender());
	} catch (std::out_of_range & e) {
		// If channel doesn't exist, create it and add the client to the channel
		Channel * new_channel = new Channel(channel_name);
		_channels.insert(std::pair<std::string, Channel *>(channel_name, new_channel));
	}

	/*std::map<std::string, Channel *>::iterator it_channel = _channels.begin();
	std::cout << "Current channels: ";
	for (; it_channel != _channels.end(); it_channel++) {
		std::cout << it_channel->first << ", ";
	}
	std::cout << std::endl;*/
}

void IRCServer::_execute_privmsg(IRCMessage & message) {
	std::cout << "commande privmsg: " << message.get_command() << std::endl;
}

void IRCServer::_execute_topic(IRCMessage & message) {
	std::cout << "commande topic: " << message.get_command() << std::endl;
	std::vector<int> receivers;
	receivers.push_back(message.get_sender());
	std::map<std::string, Channel *>::const_iterator it = find_channel(message.get_params()[0]);
	if (it != _channels.end())
	{
		std::string	reply = ERR_NOTONCHANNEL(message.get_params()[0]);
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		return ;
	}
	Channel * channel_tmp = it->second;
	if (find(channel_tmp->clients_begin(), channel_tmp->clients_end(), message.get_sender()) == channel_tmp->clients_end())
	{
		std::string	reply = ERR_NOTONCHANNEL(message.get_params()[0]);
		_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		return ;
	}
	if (message.get_params().size() > 1)
	{
		/*if (pas operator chan)
		{
			std::string	reply = ERR_CHANOPRIVSNEEDED(message.get_params()[0]);
			_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		}
		else
		{*/
			channel_tmp->set_topic(message.get_params()[1]);
			receivers.clear();
			std::vector<int>::const_iterator it_clients = channel_tmp->clients_begin();
			for (; it_clients != channel_tmp->clients_end(); it_clients++)
				receivers.push_back(*it_clients);
			std::string reply = "TOPIC " + channel_tmp->get_name() + " :" + channel_tmp->get_topic();
			_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		//}
		return ;
	}
	else
	{
		/*if (toipc présent)
		{
			std::string	reply = RPL_TOPIC(message->get_params()[0], channel_tmp->get_topic());
			_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		}
		else
		{*/
			std::string	reply = RPL_NOTOPIC(message.get_params()[0]);
			_tcp_server.messages_to_be_sent.push_back(TCPMessage(receivers, reply));
		//}
	}
}

std::map<int, IRCClient *>::const_iterator IRCServer::find_nickname(std::string & nickname) const {
	std::map<int, IRCClient *>::const_iterator it = _clients.begin();
	for (; it != _clients.end(); it++)
		if (it->second->get_nickname() == nickname)
			break;
	return it;
}

std::map<std::string, Channel *>::const_iterator IRCServer::find_channel(std::string & channel_name) const {
	std::map<std::string, Channel *>::const_iterator it = _channels.begin();
	for (; it != _channels.end(); it++)
		if (it->second->get_name() == channel_name)
			break;
	return it;
}

//void	IRCServer::_leave_channel()