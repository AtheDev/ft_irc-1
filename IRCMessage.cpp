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

const char*
	IRCMessage::Error_wrong_param_amount::what() const throw() { return ("Wrong number of params"); }

const char*
	IRCMessage::Error_invalid_nickname::what() const throw() { return ("Invalid nickname"); }

const char*
	IRCMessage::Error_invalid_username::what() const throw() { return ("Invalid username"); }

const char*
	IRCMessage::Error_invalid_usermodebyte::what() const throw() { return ("Invalid usermode byte"); }

const char*
	IRCMessage::Error_invalid_usermode::what() const throw() { return ("Invalid usermode"); }

const char*
	IRCMessage::Error_invalid_realname::what() const throw() { return ("Invalid realname"); }

const char*
	IRCMessage::Error_invalid_channel::what() const throw() { return ("Invalid channel"); }

const char*
	IRCMessage::Error_invalid_key::what() const throw() { return ("Invalid key"); }

const char*
	IRCMessage::Error_invalid_msgtarget::what() const throw() { return ("Invalid message target(s)"); }

const char*
	IRCMessage::Error_invalid_servername::what() const throw() { return ("Invalid message target(s)"); }

/**
 * @brief Check IRCMessage prefix, command and parameters sanity and assigns type to its parameters
 * throws exceptions is the formatting is wrong
 */
void
	IRCMessage::_sanity_check()
{
	if (_command == "PASS")
	{
		if (_params.size() != 1)
			throw Error_wrong_param_amount();
		//todo: is there forbidden octets for passwords?
	}
	else if (_command == "NICK")
	{
		if (_params.size() != 1)
			throw Error_wrong_param_amount();
		if (!fmatch(_params[0], NICKNAME))
			throw Error_invalid_nickname();
	}
	else if (_command == "USER")
	{
		if (_params.size() != 4)
			throw Error_wrong_param_amount();
		if (!fmatch(_params[0], USERNAME))
			throw Error_invalid_username();
		if (!fmatch(_params[1], "%(1)[0:9]")) //not 100% sure about this one (usermode)
			throw Error_invalid_usermodebyte();
		//third param is unused
		if (!fmatch(_params[3], REALNAME))
			throw Error_invalid_realname();
	}
	else if (_command == "OPER")
	{
		if (_params.size() != 2)
			throw Error_wrong_param_amount();
		if (!fmatch(_params[0], USERNAME))
			throw Error_invalid_username();
		//todo: is there forbidden octets for passwords?
	}
	else if (_command == "MODE")
	{
		if (_params.size() != 2)
			throw Error_wrong_param_amount();
		if (!fmatch(_params[0], NICKNAME))
			throw Error_invalid_nickname();
		if (!fmatch(_params[1], USERMODE))
			throw Error_invalid_usermode();
	}
	else if (_command == "QUIT")
	{
		if (_params.size() > 1)
			throw Error_wrong_param_amount();
		//todo: is there forbidden octets in quit message (probably)
	}
	else if (_command == "JOIN") //not done
	{
		if (_params.size() > 2 || _params.size() == 0)
			throw Error_wrong_param_amount();
		if (_params[0] == "0")
		{
			if (_params.size() != 1)
				throw Error_wrong_param_amount();
		}
		else
		{
			if (_params.size() != 1 || _params.size() != 2)
				throw Error_wrong_param_amount();
			if (!fmatch(_params[0], CHANNEL_LIST))
				throw Error_invalid_channel();
			//key list not really checking properly, need to split between ,
			//we get the correct result though
			if (_params.size() == 2 && !fmatch(_params[1], KEY_LIST))
				throw Error_invalid_key();
		}
	}
	else if (_command == "PART")
	{
		if (_params.size() < 1 || _params.size() > 2)
			throw Error_wrong_param_amount();
		if (!fmatch(_params[0], CHANNEL_LIST))
			throw Error_invalid_channel();
		//Is there forbidden octets in part message (probably)
	}
	else if (_command == "PRIVMSG")
	{
		if (_params.size() != 2)
			throw Error_wrong_param_amount();
		//todo: targetmask
		if (!fmatch(_params[0], MSGTARGET))
			throw Error_invalid_msgtarget();
		//Is there forbidden octets in privmsg body?
	}
	else if (_command == "PING" || _command == "PONG")
	{
		if (_params.size() == 0 || _params.size() > 2)
			throw Error_wrong_param_amount();
		if (!fmatch(_params[0], SERVERNAME))
			throw Error_invalid_servername();
		if (_params.size() == 2 && !fmatch(_params[0], SERVERNAME))
			throw Error_invalid_servername();
	}
	else if (_command == "ERROR")
	{
		if (_params.size() != 1)
			throw Error_wrong_param_amount();
		//forbidden octets in error message?
	}
}