#ifndef REPLY_HPP
#define REPLY_HPP

#include <vector>
#include <map>
#include "TCPMessage.hpp"
#include "IRCClient.hpp"
#include "Channel.hpp"
#include "Utils.hpp"


//TODO: THIS FILE CONTAINS COMMENTED PARTIALLY FORMATTED FUNCTION DECLARATION !
// Update this file when add a new make_reply function

std::string prepare_reply_command(const std::string & command, const IRCClient & client);

std::string prepare_reply_RPL_ERR(const std::string & numeric, const IRCClient & client);

TCPMessage make_reply_PRIVMSG_CHANNEL(const IRCClient & client, const Channel & channel,
									  const std::string & message);

TCPMessage make_reply_PRIVMSG_USER(const IRCClient & client, const IRCClient & client_recipient,
								   const std::string & channel_name, const std::string & message);

TCPMessage make_reply_NOTICE_CHANNEL(const IRCClient & client, const Channel & channel,
									 const std::string & message);

TCPMessage make_reply_NOTICE_USER(const IRCClient & client, const IRCClient & client_recipient,
								  const std::string & channel_name, const std::string & message);

TCPMessage make_reply_JOIN(const IRCClient & client, const Channel & channel);

TCPMessage make_reply_PART(const IRCClient & client, const Channel & channel,
						   const std::string & part_message = std::string());

TCPMessage make_reply_TOPIC(const IRCClient & client, const Channel & channel);

TCPMessage make_reply_PONG(const IRCClient & client, const std::string & servername);

TCPMessage make_reply_QUIT(const IRCClient & client, const std::string & quit_message);

TCPMessage make_reply_ERROR(const IRCClient & client, const std::string & error_message);

TCPMessage make_reply_NICK(const IRCClient & client, const std::string & new_nick, const std::vector<int> & receivers);

TCPMessage make_reply_MODE(const IRCClient & client, const Channel & channel,
							const std::string & channel_mode, const std::string & channel_key);

TCPMessage make_reply_KILL(const IRCClient & client_killer, const IRCClient & client_killed,
						   const std::string & comment);

//001
TCPMessage make_reply_RPL_WELCOME(const IRCClient & client);
//002
TCPMessage make_reply_RPL_YOURHOST(const IRCClient & client, const std::string & servername,
								   const std::string & version);
//003
TCPMessage make_reply_RPL_CREATED(const IRCClient & client, const std::string & date);
//004
TCPMessage make_reply_RPL_MYINFO(const IRCClient & client, const std::string & servername,
								 const std::string & version, const std::string & user_modes,
								 const std::string & channel_modes);
//221
TCPMessage make_reply_RPL_UMODEIS(const IRCClient & client);
//301
TCPMessage make_reply_RPL_AWAY(const IRCClient & client, const IRCClient & client_away);
//305
TCPMessage make_reply_RPL_UNAWAY(const IRCClient & client);
//306
TCPMessage make_reply_RPL_NOWAWAY(const IRCClient & client);
//311
TCPMessage make_reply_RPL_WHOISUSER(const IRCClient & client, const IRCClient & client_target);
//313
TCPMessage make_reply_RPL_WHOISOPERATOR(const IRCClient & client, const IRCClient & client_target);
//315
//TCPMessage make_reply_RPL_ENDOFWHO(const IRCClient & client, name) ("315" + name + " :End of WHO list")
//318
TCPMessage make_reply_RPL_ENDOFWHOIS(const IRCClient & client);
//319
TCPMessage make_reply_RPL_WHOISCHANNELS(const IRCClient & client, const IRCClient & client_target,
										const std::string & channels_names);
//322
TCPMessage make_reply_RPL_LIST(const IRCClient & client, const Channel & channel);
//323
TCPMessage make_reply_RPL_LISTEND(const IRCClient & client);
//324
TCPMessage make_reply_RPL_CHANNELMODEIS(const IRCClient & client, const Channel & channel);
//331
TCPMessage make_reply_RPL_NOTOPIC(const IRCClient & client, const Channel & channel);
//332
TCPMessage make_reply_RPL_TOPIC(const IRCClient & client, const Channel & channel);
//352
//TCPMessage make_reply_RPL_WHOREPLY(const IRCClient & client, channel, user, host, server, nick, hopcount, realname) ("352" + channel + " " + user + " " + host + " " + server + " " + nick + /*( "H" / "G" > ["*"] [ ( "@" / "+" ) ]*/ " :" + hopcount + " " + realname)
//353
TCPMessage make_reply_RPL_NAMREPLY(const IRCClient & client, const Channel & channel,
								   const std::string & users_list);
//366
TCPMessage make_reply_RPL_ENDOFNAMES(const IRCClient & client, const std::string & channel_name);
//367
//TCPMessage make_reply_RPL_BANLIST(const IRCClient & client, channel, bannid) ("367" + channel + " " + banmask)
//368
//TCPMessage make_reply_RPL_ENDOFBANLIST(const IRCClient & client, channel) ("368" + channel + " :End of channel ban list")
//381
TCPMessage make_reply_RPL_YOUREOPER(const IRCClient & client);


//401
TCPMessage make_reply_ERR_NOSUCHNICK(const IRCClient & client, const std::string & nickname);
//402
//TCPMessage make_reply_ERR_NOSUCHSERVER(const IRCClient & client, server_name) ("402" + server name + " :No such server")
//403
TCPMessage make_reply_ERR_NOSUCHCHANNEL(const IRCClient & client, const std::string & channel_name);
//404
TCPMessage make_reply_ERR_CANNOTSENDTOCHAN(const IRCClient & client,
										   const std::string & channel_name);
//405
TCPMessage make_reply_ERR_TOOMANYCHANNELS(const IRCClient & client, const std::string & channel_name);
//407
//TCPMessage make_reply_ERR_TOOMANYTARGETS(const IRCClient & client, target, err_code, abort_message) ("407" + target + " :" + err_code + " recipients. " + abort_message)
//409
TCPMessage make_reply_ERR_NOORIGIN(const IRCClient & client);
//411
TCPMessage make_reply_ERR_NORECIPIENT(const IRCClient & client, const std::string & command);
//412
TCPMessage make_reply_ERR_NOTEXTTOSEND(const IRCClient & client);
//413
//TCPMessage make_reply_ERR_NOTOPLEVEL(const IRCClient & client, mask) ("413" + mask + " :No toplevel domain specified")
//414
//TCPMessage make_reply_ERR_WILDTOPLEVEL(const IRCClient & client, mask) ("414" + mask + " :Wildcard in toplevel domain")
//431
TCPMessage make_reply_ERR_NONICKNAMEGIVEN(const IRCClient & client);
//432
TCPMessage make_reply_ERR_ERRONEUSNICKNAME(const IRCClient & client, const std::string & nickname);
//433
TCPMessage make_reply_ERR_NICKNAMEINUSE(const IRCClient & client, const std::string & nickname);
//436
//TCPMessage make_reply_ERR_NICKCOLLISION(const IRCClient & client,
//										const IRCClient & collided_client);
//441
TCPMessage make_reply_ERR_USERNOTINCHANNEL(const IRCClient & client, const std::string & channel_name, const std::string & target);
//442
TCPMessage make_reply_ERR_NOTONCHANNEL(const IRCClient & client, const std::string & channel_name);
//461
TCPMessage make_reply_ERR_NEEDMOREPARAMS(const IRCClient & client, const std::string & command);
//462
TCPMessage make_reply_ERR_ALREADYREGISTRED(const IRCClient & client);
//464
TCPMessage make_reply_ERR_PASSWDMISMATCH(const IRCClient & client);
//467
TCPMessage make_reply_ERR_KEYSET(const IRCClient & client, const std::string & channel_name);
//471
//TCPMessage make_reply_ERR_CHANNELISFULL(const IRCClient & client, channel) ("471" + channel + " :Cannot join channel (+l)")
//472
TCPMessage make_reply_ERR_UNKNOWNMODE(const IRCClient & client, const std::string & channel_name, const char & mode);
//473
//TCPMessage make_reply_ERR_INVITEONLYCHAN(const IRCClient & client, channel) ("473" + channel + " :Cannot join channel (+i)")
//474
//TCPMessage make_reply_ERR_BANNEDFROMCHAN(const IRCClient & client, channel) ("474" + channel + " :Cannot join channel (+b)")
//475
TCPMessage make_reply_ERR_BADCHANNELKEY(const IRCClient & client, const std::string & channel_name);
//476
TCPMessage make_reply_ERR_BADCHANMASK(const IRCClient & client, const std::string & channel_name);
//481
TCPMessage make_reply_ERR_NOPRIVILEGES(const IRCClient & client);
//482
TCPMessage make_reply_ERR_CHANOPRIVSNEEDED(const IRCClient & client,
										   const std::string & channel_name);
//483
//TCPMessage make_reply_ERR_CANTKILLSERVER(const IRCClient & client, ) ("483 :You can't kill a server!")
//491
//TCPMessage make_reply_ERR_NOOPERHOST(const IRCClient & client, ) ("491 :No O-lines for your host")
//501
TCPMessage make_reply_ERR_UMODEUNKNOWNFLAG(const IRCClient & client);
//502
TCPMessage make_reply_ERR_USERSDONTMATCH(const IRCClient & client);



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