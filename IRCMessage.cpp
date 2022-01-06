#include "IRCMessage.hpp"

IRCMessage::IRCMessage(int fd, std::string line): _fd(fd)
{std::cout << "line : " << line << std::endl;
	size_t	pos1 = 0, pos2 = 0, pos_crlf;

	try
	{
		if (line.empty())
			throw Error_message_empty();
		if ((pos_crlf = line.find("\r\n")) == line.npos)
			throw Error_message_nocrlf();
		line = line.substr(0, pos_crlf); //shorten string to simplify further code
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
	_params.push_back(line.substr(pos1));
}

IRCMessage::IRCMessage(int reply, int fd, const IRCServer & server, const IRCMessage & message) {

    std::string b("DATE"), b1("USER_MODES"), b2("CHANNEL_MODES");
    
    switch (reply) {
        case ERR_ALREADYREGISTRED: _reply = ERR_ALREADYREGISTRED();
        case ERR_NICKNAMEINUSE: _reply = ERR_NICKNAMEINUSE(message.get_params()[0]);
        case ERR_NICKCOLLISION: {
                                    std::map<int, IRCClient *>::const_iterator it = server.find_nickname(message.get_params()[0]);
                                    _reply = ERR_NICKCOLLISION(message.get_params()[0], it->second->get_username(), it->second->get_hostname());
                                }
        case RPL_WELCOME: _reply = RPL_WELCOME(server.get_clients()[fd]->get_nickname(), server.get_clients()[fd]->get_username(), server.get_clients()[fd]->get_hostname());
        case RPL_YOURHOST: _reply = RPL_YOURHOST(server.get_servername(), server.get_version());
        case RPL_CREATED: _reply = RPL_CREATED(b);
        case RPL_MYINFO: _reply = RPL_MYINFO(server.get_servername(), server.get_version(), b1, b2);
        default: break;
    }
}

/*TCPMessage &    IRCMessage::push_to_tcp_message(void) {
    
}*/

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
	IRCMessage::get_sender() const { return _fd; }

void IRCMessage::set_command(std::string cmd) { _command = cmd;}

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