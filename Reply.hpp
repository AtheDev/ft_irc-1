#ifndef REPLY_HPP
#define REPLY_HPP

#include <vector>
#include "TCPMessage.hpp"
#include "IRCClient.hpp"
#include "Channel.hpp"


//TODO: THIS FILE CONTAINS COMMENTED PARTIALLY FORMATTED FUNCTION DECLARATION !
// Update this file when add a new make_reply function

TCPMessage make_reply_JOIN(const IRCClient & client, const Channel & channel);

TCPMessage make_reply_PART(const IRCClient & client, const Channel & channel,
						   const std::string & part_message = std::string());

//001
TCPMessage make_reply_RPL_WELCOME(const IRCClient & client);
//002
TCPMessage make_reply_RPL_YOURHOST(const IRCClient & client,
								   const std::string & servername, const std::string & version);
//003
TCPMessage make_reply_RPL_CREATED(const IRCClient & client, const std::string & date);
//004
TCPMessage make_reply_RPL_MYINFO(const IRCClient & client,
								 const std::string & servername, const std::string & version,
								 const std::string & user_modes, const std::string & channel_modes);
//221
//TCPMessage make_reply_RPL_UMODEIS(const IRCClient & client, user_mode_str) ("221" + user_mode_str)
//301
//TCPMessage make_reply_RPL_AWAY(const IRCClient & client, nick, away_message) ("301" + nick + " :" + away_message)
//305
//TCPMessage make_reply_RPL_UNAWAY(const IRCClient & client, ) ("305 :You are no longer marked as being away")
//306
//TCPMessage make_reply_RPL_NOWAWAY(const IRCClient & client, ) ("306 :You have been marked as being away")
//315
//TCPMessage make_reply_RPL_ENDOFWHO(const IRCClient & client, name) ("315" + name + " :End of WHO list")
//322
//TCPMessage make_reply_RPL_LIST(const IRCClient & client, channel, visible, topic) ("322" + channel + " " +  /*#*/ visible + " :" + topic)
//323
//TCPMessage make_reply_RPL_LISTEND(const IRCClient & client, ) ("323 :End of LIST")
//324
//TCPMessage make_reply_RPL_CHANNELMODEIS(const IRCClient & client, channel, mode, mode_params) ("324" + channel + " " + mode + " " + mode params)
//331
//TCPMessage make_reply_RPL_NOTOPIC(const IRCClient & client, channel) ("331" + channel + " :No topic is set")
//332
//TCPMessage make_reply_RPL_TOPIC(const IRCClient & client, channel, topic) ("332" + channel + " :" + topic)
//352
//TCPMessage make_reply_RPL_WHOREPLY(const IRCClient & client, channel, user, host, server, nick, hopcount, realname) ("352" + channel + " " + user + " " + host + " " + server + " " + nick + /*( "H" / "G" > ["*"] [ ( "@" / "+" ) ]*/ " :" + hopcount + " " + realname)
//353
//TCPMessage make_reply_RPL_NAMREPLY(const IRCClient & client, channel, list_nick) ("353" + channel + " :"list_nick)
//366
//TCPMessage make_reply_RPL_ENDOFNAMES(const IRCClient & client, channel) ("366" + channel + " :End of NAMES list")
//367
//TCPMessage make_reply_RPL_BANLIST(const IRCClient & client, channel, bannid) ("367" + channel + " " + banmask)
//368
//TCPMessage make_reply_RPL_ENDOFBANLIST(const IRCClient & client, channel) ("368" + channel + " :End of channel ban list")
//381
//TCPMessage make_reply_RPL_YOUREOPER(const IRCClient & client, ) ("381 :You are now an IRC operator")


//401
//TCPMessage make_reply_ERR_NOSUCHNICK(const IRCClient & client, nickname) ("401" + nickname + " :No such nick/channel")
//402
//TCPMessage make_reply_ERR_NOSUCHSERVER(const IRCClient & client, server_name) ("402" + server name + " :No such server")
//403
TCPMessage make_reply_ERR_NOSUCHCHANNEL(const IRCClient & client, const std::string & channel_name);
//404
//TCPMessage make_reply_ERR_CANNOTSENDTOCHAN(const IRCClient & client, channel) ("404" + channel + " :Cannot send to channel")
//405
//TCPMessage make_reply_ERR_TOOMANYCHANNELS(const IRCClient & client, channel) ("405" + channel + " :You have joined too many channels")
//407
//TCPMessage make_reply_ERR_TOOMANYTARGETS(const IRCClient & client, target, err_code, abort_message) ("407" + target + " :" + err_code + " recipients. " + abort_message)
//411
//TCPMessage make_reply_ERR_NORECIPIENT(const IRCClient & client, command) ("411 :No recipient given (" + command + ")")
//412
//TCPMessage make_reply_ERR_NOTEXTTOSEND(const IRCClient & client, ) ("412 :No text to send")
//413
//TCPMessage make_reply_ERR_NOTOPLEVEL(const IRCClient & client, mask) ("413" + mask + " :No toplevel domain specified")
//414
//TCPMessage make_reply_ERR_WILDTOPLEVEL(const IRCClient & client, mask) ("414" + mask + " :Wildcard in toplevel domain")
//431
//TCPMessage make_reply_ERR_NONICKNAMEGIVEN(const IRCClient & client, ) ("431 :No nickname given")
//432
//TCPMessage make_reply_ERR_ERRONEUSNICKNAME(const IRCClient & client, nick) ("432" + nick + " :Erroneous nickname")
//433
TCPMessage make_reply_ERR_NICKNAMEINUSE(const IRCClient & client, const std::string & nick);
//436
TCPMessage make_reply_ERR_NICKCOLLISION(const IRCClient & client,
										const IRCClient & collided_client);
//442
TCPMessage make_reply_ERR_NOTONCHANNEL(const IRCClient & client, const std::string & channel_name);
//461
//TCPMessage make_reply_ERR_NEEDMOREPARAMS(const IRCClient & client, command) ("461" + command + " :Not enough parameters")
//462
TCPMessage make_reply_ERR_ALREADYREGISTRED(const IRCClient & client);
//464
//TCPMessage make_reply_ERR_PASSWDMISMATCH(const IRCClient & client, ) ("464 :Password incorrect")
//467
//TCPMessage make_reply_ERR_KEYSET(const IRCClient & client, channel) ("467" + channel + " :Channel key already set")
//471
//TCPMessage make_reply_ERR_CHANNELISFULL(const IRCClient & client, channel) ("471" + channel + " :Cannot join channel (+l)")
//472
//TCPMessage make_reply_ERR_UNKNOWNMODE(const IRCClient & client, char, channel) ("472" + char + " :is unknown mode char to me for " + channel)
//473
//TCPMessage make_reply_ERR_INVITEONLYCHAN(const IRCClient & client, channel) ("473" + channel + " :Cannot join channel (+i)")
//474
//TCPMessage make_reply_ERR_BANNEDFROMCHAN(const IRCClient & client, channel) ("474" + channel + " :Cannot join channel (+b)")
//475
//TCPMessage make_reply_ERR_BADCHANNELKEY(const IRCClient & client, channel) ("475" + channel + " :Cannot join channel (+k)")
//476
//TCPMessage make_reply_ERR_BADCHANMASK(const IRCClient & client, channel) ("476" + channel + " :Bad Channel Mask")
//481
//TCPMessage make_reply_ERR_NOPRIVILEGES(const IRCClient & client, ) ("481 :Permission Denied- You're not an IRC operator")
//482
//TCPMessage make_reply_ERR_CHANOPRIVSNEEDED(const IRCClient & client, channel) ("482" + channel + " :You're not channel operator")
//483
//TCPMessage make_reply_ERR_CANTKILLSERVER(const IRCClient & client, ) ("483 :You can't kill a server!")
//491
//TCPMessage make_reply_ERR_NOOPERHOST(const IRCClient & client, ) ("491 :No O-lines for your host")
//501
//TCPMessage make_reply_ERR_UMODEUNKNOWNFLAG(const IRCClient & client, ) ("501 :Unknown MODE flag")
//502
//TCPMessage make_reply_ERR_USERSDONTMATCH(const IRCClient & client, ) ("502 :Cannot change mode for other users")



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