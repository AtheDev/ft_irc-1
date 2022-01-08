#include "IRCMessage.hpp"

IRCMessage::IRCMessage(int fd, std::string line)
: _sender(fd)
{
	try { _parse_line(line); }
	catch (Error_message_empty &e) { std::cerr << "Error: message is empty" << std::endl; return ; }
	catch (Error_message_nocrlf &e) { std::cerr << "Error: message has no crlf" << std::endl; return ; }
	catch (Error_message_invalid_prefix &e) { std::cerr << "Error: message has invalid prefix" << std::endl; return ; }
}

IRCMessage::IRCMessage(int reply, const IRCServer &server, const IRCMessage &message)
{
	std::string date("DATE"), user_modes("USER_MODES"), channel_modes("CHANNEL_MODES");

	switch (reply)
	{
		case ERR_ALREADYREGISTERED: _reply = ERR_ALREADYREGISTERED(); break;
		case ERR_NICKNAMEINUSE: _reply = ERR_NICKNAMEINUSE(message.get_params()[0]); break;
		case ERR_NICKCOLLISION: {
			std::map<int, IRCClient*>::const_iterator it = server.find_nickname(message.get_params()[0]);
			_reply = ERR_NICKCOLLISION(message.get_params()[0], it->second->get_username(), it->second->get_hostname());
			break;
		}
		case RPL_WELCOME: _reply = RPL_WELCOME(	server.get_clients()[message.get_sender()]->get_nickname(),
												server.get_clients()[message.get_sender()]->get_username(),
												server.get_clients()[message.get_sender()]->get_hostname()); break;
		case RPL_YOURHOST: _reply = RPL_YOURHOST(server.get_servername(), server.get_version()); break;
		case RPL_CREATED: _reply = RPL_CREATED(date); break;
		case RPL_MYINFO: _reply = RPL_MYINFO(server.get_servername(), server.get_version(), user_modes, channel_modes); break;
		default: break;
	}
	_receivers.push_back(message.get_sender());
}

IRCMessage::IRCMessage(TCPMessage &tcpmessage)
{
	_sender = tcpmessage.get_sender();
	_receivers = tcpmessage.get_receivers();
	try { _parse_line(tcpmessage.get_payload()); }
	catch (Error_message_empty &e) { std::cerr << "Error: message is empty" << std::endl; return ; }
	catch (Error_message_nocrlf &e) { std::cerr << "Error: message has no crlf" << std::endl; return ; }
	catch (Error_message_invalid_prefix &e) { std::cerr << "Error: message has invalid prefix" << std::endl; return ; }
}

TCPMessage
	IRCMessage::to_tcp_message() { return TCPMessage(_receivers, _reply); }

IRCMessage::~IRCMessage() {}

bool
	IRCMessage::empty() const { return _command.empty(); }

bool
	IRCMessage::has_prefix() const { return !_prefix.empty(); }

std::string
	IRCMessage::get_prefix() const { return _prefix; }

std::string
	IRCMessage::get_command() const { return _command; }

std::vector<std::string>
	IRCMessage::get_params() const { return _params; }

int
	IRCMessage::get_sender() const { return _sender; }

std::vector<int>
	IRCMessage::get_receivers() const { return _receivers; }

void IRCMessage::set_command(std::string cmd) { _command = cmd; }

std::ostream
	&operator<<(std::ostream &o, const IRCMessage &i)
{
	if (i.has_prefix())
		o << "prefix:\t|" << i.get_prefix() << '|' << std::endl;
	else
		o << "no prefix" << std::endl;
	o << "command:\t|" << i.get_command() << '|' << std::endl;
	if (i.get_params().empty())
		o << "no params";
	else
	{
		o << "params:\t";
		for (size_t j = 0; j < i.get_params().size(); ++j)
			o << '|' << i.get_params()[j] << "| ";
	}
	o << std::endl;
	return (o);
}

const char*
	IRCMessage::Error_message_empty::what() const throw() { return ("Empty message"); }

const char*
	IRCMessage::Error_message_nocrlf::what() const throw() { return ("No crlf"); }

const char*
	IRCMessage::Error_message_invalid_prefix::what() const throw() { return ("Invalid prefix"); }

void
	IRCMessage::_parse_line(std::string line)
{
	size_t	pos1 = 0, pos2 = 0;

	try
	{
		if (line.empty())
			throw Error_message_empty();
		//here we get optional prefix
		if (line[0] == ':')
		{
			++pos1;
			if (line[pos1] == ' ' || line[pos1] == '\0')
				throw Error_message_invalid_prefix();
			pos2 = line.find(' ');
			//error if pos2 == line.npos
			_prefix = line.substr(pos1, pos2 - pos1);
			pos1 = pos2 + 1;
		}
	}
	catch (Error_message_empty &e) { std::cerr << "Error: message is empty" << std::endl; return ; }
	catch (Error_message_nocrlf &e) { std::cerr << "Error: message has no crlf" << std::endl; return ; }
	catch (Error_message_invalid_prefix &e) { std::cerr << "Error: message has invalid prefix" << std::endl; return ; }
	//here we get command
	if ((pos2 = line.find(' ', pos1)) == line.npos)
	{
		_command = line.substr(pos1);
		return ;
	}
	_command = line.substr(pos1, pos2 - pos1);
	pos1 = pos2 + 1;
	//here we get optional parameters
	while ((pos2 = line.find(' ', pos1 + 1)) != line.npos)
	{
		if (line[pos1] == ':')
		{
			_params.push_back(line.substr(pos1 + 1));
			return ;
		}
		_params.push_back(line.substr(pos1, pos2 - pos1));
		pos1 = pos2 + 1;
	}
	if (line[pos1] == ':')
		pos1++;
	_params.push_back(line.substr(pos1));
}