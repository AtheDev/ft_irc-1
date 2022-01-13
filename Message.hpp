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
# define	NUMBER		"[0-9]"
# define	SHORTNAME	"%(1-1)" LETTER "+" NUMBER "%(-)" LETTER "+" NUMBER "+-" "%(-)" LETTER "+" NUMBER

#endif