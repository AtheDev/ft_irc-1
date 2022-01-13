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

//Parsing bnf

# define	LETTER		"[a-z]+[A-Z]"
# define	DIGIT		"[0-9]"
# define	SPECIAL		"[%91%-%96%]+[%123%-%125%]"

# define	NICKNAME	"%(1-1)" LETTER "+" SPECIAL "%(-8)" LETTER "+" DIGIT "+" SPECIAL "+-"
# define	REALNAME	"%(1-1)" LETTER "%(-)" LETTER "+-+ "
//Not clear in rfc2812, I assume REALNAME is only letters, but with spaces and dashes allowed except for first char
# define	USERNAME	"%(1-)[%1%-%9%]+[%11%-%12%]+[%14%-%31%]+[%33%-%63%]+[%65%-%255%]"
# define	USERMODE	"%R(-)%(1-1)++-%(-)i+w+o+O+r%R"
//# define	CHANSTRING	"%(1-1)[%1%-%9%]+[%11%-%12%]+[%14%-%31%]+[%33%-%43%]+[%45%-%57%]+[%59%-%255%]"
//# define	CHANNELID	"%(5-5)[A-Z]+" DIGIT
//# define	CHANNEL		"%(1-1)#+++&" CHANSTRING "%R(0-1)%(1-1):" CHANSTRING "%R"

#endif