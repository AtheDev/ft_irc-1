#include "IRCMessage.hpp"
#include "Message.hpp"

bool fmatch(std::string token, std::string format);

IRCMessage::IRCMessage(const TCPMessage &tcpmessage)
{
	_sender = tcpmessage.get_sender();
	_receivers = tcpmessage.get_receivers();
	try { _parse_line(tcpmessage.get_payload()); }
	catch (Error_message_empty &e) { std::cerr << "Error: message is empty" << std::endl; return ; }
	catch (Error_message_nocrlf &e) { std::cerr << "Error: message has no crlf" << std::endl; return ; }
	catch (Error_message_invalid_prefix &e) { std::cerr << "Error: message has invalid prefix" << std::endl; return ; }
}

IRCMessage::IRCMessage(std::string line)
{
	try { _parse_line(line); }
	catch (Error_message_empty &e) { std::cerr << "Error: message is empty" << std::endl; return ; }
	catch (Error_message_nocrlf &e) { std::cerr << "Error: message has no crlf" << std::endl; return ; }
	catch (Error_message_invalid_prefix &e) { std::cerr << "Error: message has invalid prefix" << std::endl; return ; }
}

IRCMessage::~IRCMessage() {}

bool
	IRCMessage::empty() const { return _command.empty(); }

bool
	IRCMessage::has_prefix() const { return !_prefix.empty(); }

const std::string &
	IRCMessage::get_prefix() const { return _prefix; }

const std::string &
	IRCMessage::get_command() const { return _command; }

const std::vector<std::string> &
	IRCMessage::get_params() const { return _params; }

int
	IRCMessage::get_sender() const { return _sender; }

std::vector<int>
	IRCMessage::get_receivers() const { return _receivers; }

void IRCMessage::set_command(std::string cmd) { _command = cmd; }

/**
 * @brief Debug function to display IRCMessage content after parsing
 */
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

/**
 * @brief Parse a string into prefix, command and parameters according to rfc2812
 */
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
			//error if pos2 == line.npos?
			_prefix = line.substr(pos1, pos2 - pos1);
			pos1 = pos2 + 1;
		}
	}
	catch (Error_message_empty &e) { std::cerr << "Error: message is empty" << std::endl; return ; } //this should be ignored, but not here most likely
	catch (Error_message_nocrlf &e) { std::cerr << "Error: message has no crlf" << std::endl; return ; } //this is useless
	catch (Error_message_invalid_prefix &e) { std::cerr << "Error: message has invalid prefix" << std::endl; return ; }
	//here we get command
	//todo: error when there is no command?
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

/**
 * @brief Check IRCMessage prefix, command and parameters sanity
 * throws exceptions if the formatting is wrong
 */
int
	IRCMessage::sanity_check() const
{
	if (_command == "PASS")
	{
		if (_params.size() != 1)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "NICK")
	{
		if (_params.size() != 1)
			return ERR_NONICKNAMEGIVEN;
		if (!fmatch(_params[0], NICKNAME))
			return ERR_ERRONEUSNICKNAME;
		return OK;
	}
	else if (_command == "USER")
	{
		if (_params.size() != 4)
			return ERR_NEEDMOREPARAMS;
		if (!fmatch(_params[0], USERNAME))
			return ERR_ERROR;
		if (!fmatch(_params[1], HOSTNAME))
			return ERR_ERROR;
		if (!fmatch(_params[2], SERVERNAME))
			return ERR_ERROR;
		if (!fmatch(_params[3], REALNAME))
			return ERR_ERROR;
		return OK;
	}
	else if (_command == "OPER")
	{
		if (_params.size() != 2)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	/*else if (_command == "MODE")
	{
		if (_params.size() != 2)
			return ERR_INVALID_PARAM_AMOUNT;
		if (!fmatch(_params[0], NICKNAME))
			return ERR_INVALID_NICKNAME;
		if (!fmatch(_params[1], USERMODE))
			return ERR_INVALID_USERMODE;
		return OK;
	}*/
	else if (_command == "QUIT")
	{
		if (_params.size() > 1)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "JOIN")
	{
		if (_params.size() > 2 || _params.size() == 0)
			return ERR_NEEDMOREPARAMS;
		if (!fmatch(_params[0], CHANNEL_LIST))
			return ERR_BADCHANMASK;
		if (_params.size() == 2 && !fmatch(_params[1], KEY_LIST))
			return ERR_BADCHANMASK;
		return OK;
	}
	else if (_command == "PART")
	{
		if (_params.size() < 1 || _params.size() > 2)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "PRIVMSG")
	{
		if (_params.size() > 2)
			return ERR_NEEDMOREPARAMS;
		else if (_params.size() == 0)
			return ERR_NORECIPIENT;
		else if (_params.size() == 1)
			return ERR_NOTEXTTOSEND;
		return OK;
	}
	else if (_command == "PING" || _command == "PONG")
	{
		if (_params.size() == 0)
			return ERR_NOORIGIN;
		if (_params.size() > 2)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "NOTICE")
	{
		if (_params.size() != 2)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "TOPIC")
	{
		if (_params.size() == 0 || _params.size() > 2)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "NAMES" || _command == "LIST")
	{
		//I assume there must be no target bc we don't do multiserver
		if (_params.size() > 1)
			return ERR_NEEDMOREPARAMS;
		return OK;
	}
	else if (_command == "AWAY")
	{
		if (_params.size() > 1)
			return ERR_NEEDMOREPARAMS;
		//I assume there is no forbidden octets for away message
		return OK;
	}
	// ============= PEUT ETRE RETIRE SI ON NE FAIT PAS ==================
	else if (_command == "WHOIS")
	{
		//todo
		return OK;
	}
	else
		return KO;	
}