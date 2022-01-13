#ifndef REPLY_HPP
#define REPLY_HPP

enum    e_rpl_reply {
    RPL_WELCOME,
    RPL_YOURHOST,
    RPL_CREATED,
    RPL_MYINFO,
    RPL_UMODEIS,
    RPL_AWAY,
    RPL_UNAWAY,
    RPL_NOWAWAY,
    RPL_ENDOFWHO,
    RPL_LIST,
    RPL_LISTEND,
    RPL_CHANNELMODEIS,
    RPL_NOTOPIC,
    RPL_TOPIC,
    RPL_WHOREPLY,
    RPL_NAMREPLY,
    RPL_ENDOFNAMES,
    RPL_BANLIST,
    RPL_ENDOFBANLIST,
    RPL_YOUREOPER
};

enum    e_err_reply {
    ERR_NOSUCHNICK = 401,
    ERR_NOSUCHSERVER,
    ERR_NOSUCHCHANNEL,
    ERR_CANNOTSENDTOCHAN,
    ERR_TOOMANYCHANNELS,
    ERR_TOOMANYTARGETS,
    ERR_NORECIPIENT,
    ERR_NOTEXTTOSEND,
    ERR_NOTOPLEVEL,
    ERR_NONICKNAMEGIVEN,
    ERR_ERRONEUSNICKNAME,
    ERR_NICKNAMEINUSE,
    ERR_NICKCOLLISION,
    ERR_NOTONCHANNEL,
    ERR_NEEDMOREPARAMS,
    ERR_ALREADYREGISTRED,
    ERR_PASSWDMISMATCH,
    ERR_KEYSET,
    ERR_CHANNELISFULL,
    ERR_UNKNOWNMODE,
    ERR_INVITEONLYCHAN,
    ERR_BANNEDFROMCHAN,
    ERR_BADCHANNELKEY,
    ERR_BADCHANMASK,
    ERR_NOPRIVILEGES,
    ERR_CHANOPRIVSNEEDED,
    ERR_CANTKILLSERVER,
    ERR_NOOPERHOST,
    ERR_UMODEUNKNOWNFLAG,
    ERR_USERSDONTMATCH
};

//001
#define RPL_WELCOME(nick, user, host) ("001 :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host)
//002
#define RPL_YOURHOST(servername, version) ("002 :Your host is " + servername + ", running version " + version)
//003
#define RPL_CREATED(date) ("003 :This server was created " + date)
//004
#define RPL_MYINFO(servername, version, user_modes, channel_modes) ("004 :" + servername + " " + version + " " + user_modes + " " + channel_modes)
//221
#define RPL_UMODEIS(user_mode_str) ("221" + user_mode_str)
//301
#define RPL_AWAY(nick, away_message) ("301" + nick + " :" + away_message)
//305
#define RPL_UNAWAY() ("305 :You are no longer marked as being away")
//306
#define RPL_NOWAWAY() ("306 :You have been marked as being away")
//315
#define RPL_ENDOFWHO(name) ("315" + name + " :End of WHO list")
//322
#define RPL_LIST(channel, visible, topic) ("322" + channel + " " +  /*#*/ visible + " :" + topic)
//323
#define RPL_LISTEND() ("323 :End of LIST")
//324
#define RPL_CHANNELMODEIS(channel, mode, mode_params) ("324" + channel + " " + mode + " " + mode params)
//331
#define RPL_NOTOPIC(channel) ("331" + channel + " :No topic is set")
//332
#define RPL_TOPIC(channel, topic) ("332" + channel + " :" + topic)
//352
#define RPL_WHOREPLY(channel, user, host, server, nick, hopcount, realname) ("352" + channel + " " + user + " " + host + " " + server + " " + nick + /*( "H" / "G" > ["*"] [ ( "@" / "+" ) ]*/ " :" + hopcount + " " + realname)
//353
#define RPL_NAMREPLY(channel, list_nick) ("353" + channel + " :"list_nick)
//366
#define RPL_ENDOFNAMES(channel) ("366" + channel + " :End of NAMES list")
//367
#define RPL_BANLIST(channel, bannid) ("367" + channel + " " + banmask)
//368
#define RPL_ENDOFBANLIST(channel) ("368" + channel + " :End of channel ban list")
//381
#define RPL_YOUREOPER() ("381 :You are now an IRC operator")


//401
#define ERR_NOSUCHNICK(nickname) ("401" + nickname + " :No such nick/channel")
//402
#define ERR_NOSUCHSERVER(server_name) ("402" + server name + " :No such server")
//403
#define ERR_NOSUCHCHANNEL(channel) ("403" + channel + " :No such channel")
//404
#define ERR_CANNOTSENDTOCHAN(channel) ("404" + channel + " :Cannot send to channel")
//405
#define ERR_TOOMANYCHANNELS(channel) ("405" + channel + " :You have joined too many channels")
//407
#define ERR_TOOMANYTARGETS(target, err_code, abort_message) ("407" + target + " :" + err_code + " recipients. " + abort_message)
//411
#define ERR_NORECIPIENT(command) ("411 :No recipient given (" + command + ")")
//412
#define ERR_NOTEXTTOSEND() ("412 :No text to send")
//413
#define ERR_NOTOPLEVEL(mask) ("413" + mask + " :No toplevel domain specified")
//414
#define ERR_WILDTOPLEVEL(mask) ("414" + mask + " :Wildcard in toplevel domain")
//431
#define ERR_NONICKNAMEGIVEN() ("431 :No nickname given")
//432
#define ERR_ERRONEUSNICKNAME(nick) ("432" + nick + " :Erroneous nickname")
//433
#define ERR_NICKNAMEINUSE(nick) ("433" + nick + " :Nickname is already in use")
//436
#define ERR_NICKCOLLISION(nick, user, host) ("436" + nick + " :Nickname collision KILL from " + user + "@" + host)
//442
#define ERR_NOTONCHANNEL(channel) ("442" + channel + " :You're not on that channel")
//461
#define ERR_NEEDMOREPARAMS(command) ("461" + command + " :Not enough parameters")
//462
#define ERR_ALREADYREGISTRED() ("462 :Unauthorized command (already registered)")
//464
#define ERR_PASSWDMISMATCH() ("464 :Password incorrect")
//467
#define ERR_KEYSET(channel) ("467" + channel + " :Channel key already set")
//471
#define ERR_CHANNELISFULL(channel) ("471" + channel + " :Cannot join channel (+l)")
//472
#define ERR_UNKNOWNMODE(char, channel) ("472" + char + " :is unknown mode char to me for " + channel)
//473
#define ERR_INVITEONLYCHAN(channel) ("473" + channel + " :Cannot join channel (+i)")
//474
#define ERR_BANNEDFROMCHAN(channel) ("474" + channel + " :Cannot join channel (+b)")
//475
#define ERR_BADCHANNELKEY(channel) ("475" + channel + " :Cannot join channel (+k)")
//476
#define ERR_BADCHANMASK(channel) ("476" + channel + " :Bad Channel Mask")
//481
#define ERR_NOPRIVILEGES() ("481 :Permission Denied- You're not an IRC operator")
//482
#define ERR_CHANOPRIVSNEEDED(channel) ("482" + channel + " :You're not channel operator")
//483
#define ERR_CANTKILLSERVER() ("483 :You can't kill a server!")
//491
#define ERR_NOOPERHOST() ("491 :No O-lines for your host")
//501
#define ERR_UMODEUNKNOWNFLAG() ("501 :Unknown MODE flag")
//502
#define ERR_USERSDONTMATCH() ("502 :Cannot change mode for other users")



/*
==> PASS
==> NICK
==> USER
==> OPER
==> JOIN
==> PART
==> MODE
==> TOPIC
==> NAMES
==> LIST
==> KILL
==> KICK
==> PRIVMSG
==> NOTICE
==> WHO
==> AWAY
==> QUIT
*/

//(==> PING - ERR_NOORIGIN)
//(==> ERROR)

#endif