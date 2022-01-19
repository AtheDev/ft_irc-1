#ifndef MESSAGE_HPP
# define MESSAGE_HPP

//Connection registration

# define CMD_PASS			"PASS"
# define CMD_NICK			"NICK"
# define CMD_USER			"USER"
# define CMD_OPER			"OPER"
# define CMD_MODE			"MODE"
# define CMD_QUIT			"QUIT"

//Channel operations

# define CMD_JOIN			"JOIN"
# define CMD_PART			"PART"

//Sending messages

# define CMD_PRIVMSG		"PRIVMSG"

//Miscellaneous messages

# define CMD_PING			"PING"
# define CMD_PONG			"PONG"
# define CMD_ERROR			"ERROR"

//rfc2812 bnf helpers

# define LETTER				"[a:z]+[A-Z]"
# define DIGIT				"[0:9]"
# define SPECIAL			"[%0x5B:%0x60]+[%0x7B:%0x7D]"

# define NICKNAME			"%" LETTER "+" SPECIAL "%(:8)" LETTER "+" DIGIT "+" SPECIAL "+-"
# define USERNAME			"%(1:)[%0x01:%0x09]+[%0x0B:%0x0C]+[%0x0E:%0x1F]+[%0x21:%0x3F]+[%0x41:%0xFF]"
# define REALNAME			"%" LETTER "%(:)" LETTER "+ "
//I assume real name start with a letter and then any letter and spaces
# define USERMODE			"*((:)[%0x2B+-%(:)i+w+o+O+r])"
# define CHANSTRING			"%(1:)[%0x01:%0x09]+[%0x0B:%0x0C]+[%0x0E:%0x1F]+[%0x21:%0x2B]+[%0x2D:%0x39]+[%0x3B:%0xFF]"
//It's not clear in rfc2812 but I assume a CHANSTRING is a least one char long
# define CHANNELID			"%(5)[A:Z]+" DIGIT
# define CHANNEL			"*((1)[#+%0x2B+&]+(1)[!" CHANNELID "])" CHANSTRING "*((0:1)[:" CHANSTRING "])"
# define KEY				"%(1:23)[%0x01:%0x05]+[%0x07:%0x08]+%0x0C+[%0x0E:%0x1F]+[%0x21:%0x7F]"

# define CHANNEL_LIST		CHANNEL "*((:)[," CHANNEL "])"
# define KEY_LIST			KEY "*((:)[," KEY "])"


#endif