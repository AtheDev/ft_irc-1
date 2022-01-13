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
	_commands["PART"] = &IRCServer::_execute_part;
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

std::map<int, IRCClient *>  IRCServer::get_clients() const { return _clients;}

std::string const &			IRCServer::get_servername() const { return _servername;}

std::string const &			IRCServer::get_version() const {return _version;}

std::string const &			IRCServer::get_server_creation_date() const {return _server_creation_date;}

void IRCServer::_run() {
	while (true) {
		// Appel à update
		try {
			_tcp_server.update();
		} catch (TCPServer::ErrorSignalException & e) {
			throw e;
		}
		// Récupérer les nouveaux clients et supprimer les clients déconnectés
		_add_clients(_tcp_server.new_clients);
		_remove_clients(_tcp_server.disconnected_clients);

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
		_remove_client_from_all_channels(*it_client);
		delete _clients[*it_client];
		_clients.erase(*it_client);
	}
}

void IRCServer::_remove_client_from_all_channels(int client_socketfd) {
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
		std::cout << "Command not found: " << message.get_command() << std::endl;
}

void IRCServer::_execute_pass(IRCMessage & message) {
	std::cout << "Executing PASS: " << message.get_command() << std::endl;
	IRCClient * client = _clients.at(message.get_sender());
	if (client->get_status() == UNREGISTERED) {
		client->set_password(message.get_params()[0]);
		client->set_status(PASSWORD);
	}
	else {
		_tcp_server.messages_to_be_sent.push_back(make_reply_ERR_ALREADYREGISTRED(*client));
	}
}

void IRCServer::_execute_nick(IRCMessage & message) {
	std::cout << "Executing NICK: " << message.get_command() << std::endl;
	IRCClient * client = _clients.at(message.get_sender());
	std::string nick = message.get_params()[0];

	std::map<int, IRCClient *>::const_iterator it_client;
	if (client->get_nickname() == nick) {
		TCPMessage reply = make_reply_ERR_NICKNAMEINUSE(*client, nick);
		_tcp_server.messages_to_be_sent.push_back(reply);
	}
	else if ((it_client = find_nickname(nick)) != _clients.end()) {
		IRCClient * collided_client = it_client->second;
		TCPMessage reply = make_reply_ERR_NICKCOLLISION(*client, *collided_client);
		_tcp_server.messages_to_be_sent.push_back(reply);
	}
	else {
		client->set_nickname(message.get_params()[0]);
		if (client->get_status() == PASSWORD || client->get_status() == UNREGISTERED) {
			client->set_status(NICKNAME);
		}
	}
}

void IRCServer::_execute_user(IRCMessage & message) {
	std::cout << "Executing USER: " << message.get_command() << std::endl;
	IRCClient * client = _clients.at(message.get_sender());
	int status = client->get_status();

	if (status == REGISTERED)
	{
		TCPMessage reply = make_reply_ERR_ALREADYREGISTRED(*client);
		_tcp_server.messages_to_be_sent.push_back(reply);
	}
	else if (status == NICKNAME)
	{
		client->set_username(message.get_params()[0]);
		client->set_hostname(message.get_params()[1]); // TODO: To change ?
		client->set_realname(message.get_params()[3]);
		client->set_status(REGISTERED);
		// TODO: To change ?
		std::string user_modes("USER_MODES"), channel_modes("CHANNEL_MODES");

		TCPMessage reply = make_reply_RPL_WELCOME(*client);
		_tcp_server.messages_to_be_sent.push_back(reply);
		reply = make_reply_RPL_YOURHOST(*client, _servername, _version);
		_tcp_server.messages_to_be_sent.push_back(reply);
		reply = make_reply_RPL_CREATED(*client, _server_creation_date);
		_tcp_server.messages_to_be_sent.push_back(reply);
		reply = make_reply_RPL_MYINFO(*client, _servername, _version, user_modes, channel_modes);
		_tcp_server.messages_to_be_sent.push_back(reply);
	}
}

void IRCServer::_execute_quit(IRCMessage & message) {

	std::cout << "Executing QUIT: " << message.get_command() << std::endl;
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
	for (; it != client->get_channels().end(); it++)
	{
        Channel * channel = _channels[*it];
        //supprimer la std::string de channel du client 
        client->quit_channel(channel->get_name());
        //construire le message
        _tcp_server.messages_to_be_sent.push_back(make_reply_PART(client, *channel, quit_message));
        //supprimer le client du channel
        channel->remove_client(message.get_sender());
        //si opérateur de channel le supprimer
	    std::vector<int>::iterator it_op = channel->channel_op_begin();
	    for (; it_op != channel->channel_op_end(); it_op++)
        {
            if (*it_op == message.get_sender())
            {
                channel->get_channel_op().erase(it_op);
                break ;
            }
        }
        //si dernier client de ce channel ==> supprimer le chanel
        if (channel->get_clients().empty())
        {
            delete channel;
            _channels.erase(*it);
        }
	}
}

/**
 * @brief Executes a JOIN command.
 * @param message The message containing the JOIN command.
 */
void IRCServer::_execute_join(IRCMessage & message) {
	std::cout << "Executing JOIN: " << message.get_command() << std::endl;
	//TODO: For now, it doesn't use keys and can only manage a single channel.
	//TODO: Manage errors !
	IRCClient * client = _clients.at(message.get_sender());
	std::string channel_name = message.get_params()[0];
	Channel * channel;
	try {
		// Get the channel
		channel = _channels.at(channel_name);
		std::cout << "Channel exists" << std::endl;
	} catch (std::out_of_range & e) {
		// If it doesn't exist, create one
		channel = new Channel(channel_name);
		_channels.insert(std::pair<std::string, Channel *>(channel_name, channel));
		std::cout << "Channel was created" << std::endl;
	}
	channel->add_client(client->get_fd());
	TCPMessage reply = make_reply_JOIN(*client, *channel);
	_tcp_server.messages_to_be_sent.push_back(reply);
	std::cout << *_channels.at(channel_name) << std::endl;
}

/**
 * @brief Executes a PART command.
 * @param message The message containing the PART command.
 */
void IRCServer::_execute_part(IRCMessage & message) {
	std::cout << "Executing PART: " << message.get_command() << std::endl;
	//TODO: Manage multiple channel part
	//TODO: Manage part message. How to get the part message ?
	IRCClient * client = _clients.at(message.get_sender());
	std::string channel_name = message.get_params()[0];

	try {
		Channel * channel = _channels.at(channel_name);
		if (!channel->remove_client(message.get_sender())) {
			// If client isn't on the channel, send NOTONCHANNEL
			TCPMessage reply = make_reply_ERR_NOTONCHANNEL(*client, channel_name);
			_tcp_server.messages_to_be_sent.push_back(reply);
		} else {
			// Else, broadcast to channel's users that a new user joined the channel
			TCPMessage reply = make_reply_PART(*client, *channel);
			_tcp_server.messages_to_be_sent.push_back(reply);
		}
	} catch (std::out_of_range & e) {
		// If channel doesn't exist, send NOSUCHCHANNEL
		TCPMessage reply = make_reply_ERR_NOSUCHCHANNEL(*client, channel_name);
		_tcp_server.messages_to_be_sent.push_back(reply);
	}
	std::cout << *_channels.at(channel_name) << std::endl;
}

void IRCServer::_execute_privmsg(IRCMessage & message) {
	std::cout << "Executing PRIVMSG: " << message.get_command() << std::endl;
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
