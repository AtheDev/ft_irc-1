#include "Reply.hpp"


std::string prepare_reply_command(const std::string & command, const IRCClient & client) {
	return ":" + client.get_prefix() + " " + command + " ";
}

std::string prepare_reply_RPL_ERR(const std::string & numeric, const IRCClient & client) {
	return ":" + client.get_hostname() + " " + numeric + " " + client.get_nickname() + " ";
}

TCPMessage make_reply_PRIVMSG_CHANNEL(const IRCClient & client, const Channel & channel,
									  const std::string & message) {
	std::vector<int> receivers = channel.get_clients();
	receivers.erase(find(receivers.begin(), receivers.end(), client.get_fd()));
	std::string payload = prepare_reply_command("PRIVMSG", client);
	payload += channel.get_name() + " :" + message;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_PRIVMSG_USER(const IRCClient & client, const IRCClient & client_recipient,
								   const std::string & channel_name, const std::string & message) {
	std::vector<int> receivers(1u, client_recipient.get_fd());
	std::string payload = prepare_reply_command("PRIVMSG", client);
	payload += channel_name + " :" + message;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_NOTICE_CHANNEL(const IRCClient & client, const Channel & channel,
									 const std::string & message) {
	std::vector<int> receivers = channel.get_clients();
	receivers.erase(find(receivers.begin(), receivers.end(), client.get_fd()));
	std::string payload = prepare_reply_command("NOTICE", client);
	payload += channel.get_name() + " :" + message;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_NOTICE_USER(const IRCClient & client, const IRCClient & client_recipient,
								  const std::string & channel_name, const std::string & message) {
	std::vector<int> receivers(1u, client_recipient.get_fd());
	std::string payload = prepare_reply_command("NOTICE", client);
	payload += channel_name + " :" + message;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_JOIN(const IRCClient & client, const Channel & channel) {
	const std::vector<int> & receivers = channel.get_clients();
	std::string payload = prepare_reply_command("JOIN", client) + channel.get_name();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_PART(const IRCClient & client, const Channel & channel,
						   const std::string & part_message) {
	const std::vector<int> & receivers = channel.get_clients();
	std::string payload = prepare_reply_command("PART", client) + channel.get_name();
	if (!part_message.empty()) {
		payload += " :" + part_message;
	} else {
		payload += " :" + client.get_nickname();
	}
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_TOPIC(const IRCClient & client, const Channel & channel) {
	const std::vector<int> & receivers = channel.get_clients();
	std::string payload = prepare_reply_command("TOPIC", client);
	payload += channel.get_name() + " :" + channel.get_topic();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_PONG(const IRCClient & client, const std::string & servername) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_command("PONG", client) + servername;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_QUIT(const IRCClient & client, const std::string & quit_message) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_command("QUIT", client) + ":" + quit_message;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERROR(const IRCClient & client, const std::string & error_message) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_command("ERROR", client) + ":" + error_message;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_NICK(const IRCClient & client, const std::string & new_nick, const std::vector<int> & receivers_nick) {
	std::vector<int> receivers = receivers_nick;
	std::string payload = prepare_reply_command("NICK", client) + " :" + new_nick;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_MODE(const IRCClient & client, const Channel & channel,
							const std::string & channel_mode, const std::string & channel_key) {
	const std::vector<int> & receivers = channel.get_clients();
	std::string payload = prepare_reply_command("MODE", client) + channel.get_name();
	payload += " " + channel_mode + " " + channel_key;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_WELCOME(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("001", client);
	payload += ":Welcome to the Internet Relay Network ";
	payload += client.get_nickname() + "!";
	payload += client.get_username() + "@";
	payload += client.get_hostname();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_YOURHOST(const IRCClient & client, const std::string & servername,
								   const std::string & version) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("002", client);
	payload += ":Your host is " + servername + ", running version " + version;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_CREATED(const IRCClient & client, const std::string & date) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("003", client);
	payload += ":This server was created " + date;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_MYINFO(const IRCClient & client, const std::string & servername,
								 const std::string & version, const std::string & user_modes,
								 const std::string & channel_modes) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("004", client);
	payload += ":" + servername + " " + version + " ";
	payload += user_modes + " " + channel_modes;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_UMODEIS(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("221", client);
	if (!client.get_mode().empty()) {
		payload += "+" + client.get_mode();
	} else {
		payload += "none";;
	}
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_AWAY(const IRCClient & client, const IRCClient & client_away) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("301", client);
	payload += client_away.get_nickname() + " :" + client_away.get_away_message();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_UNAWAY(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("305", client);
	payload += ":You are no longer marked as being away";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_NOWAWAY(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("306", client);
	payload += ":You have been marked as being away";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_WHOISUSER(const IRCClient & client, const IRCClient & client_target) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload;
	payload = prepare_reply_RPL_ERR("311", client);
	payload += client_target.get_nickname() + " " + client_target.get_username() + " ";
	payload += client_target.get_hostname() + " * :" + client_target.get_realname();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_WHOISOPERATOR(const IRCClient & client, const IRCClient & client_target) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("313", client);
	payload += client_target.get_nickname() + " :is an IRC operator";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_ENDOFWHOIS(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("318", client) + ":End of WHOIS list";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_WHOISCHANNELS(const IRCClient & client, const IRCClient & client_target,
										const std::string & channels_names) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("319", client);
	//TODO: to be verified
	payload += client_target.get_nickname() + " :" + channels_names;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_LIST(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("322", client);
	payload += channel.get_name() + " " + to_string(channel.get_clients().size());
	payload += " :" + channel.get_topic();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_LISTEND(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("323", client) + ":End of LIST";
	return TCPMessage(receivers, payload);
}

//TODO: to be verified
TCPMessage make_reply_RPL_CHANNELMODEIS(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("324", client);
	payload += channel.get_name();
	if (!channel.get_mode().empty())
		payload += " +" + channel.get_mode() + " " + channel.get_key();
	else
		payload += " none";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_NOTOPIC(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("331", client);
	payload += channel.get_name() + " :No topic is set";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_TOPIC(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("332", client);
	payload += channel.get_name() + " :" + channel.get_topic();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_NAMREPLY(const IRCClient & client, const Channel & channel,
								   const std::string & users_list) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("353", client);
	payload += "= " + channel.get_name() + " :" + users_list;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_ENDOFNAMES(const IRCClient & client, const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("366", client);
	payload += channel_name + " :End of NAMES list";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_YOUREOPER(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("381", client) + ":You are now an IRC operator";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NOSUCHNICK(const IRCClient & client, const std::string & nickname) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("401", client);
	payload += nickname + " :No such nick/channel";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NOSUCHCHANNEL(const IRCClient & client,
										const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("403", client);
	payload += channel_name + " :No such channel";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_CANNOTSENDTOCHAN(const IRCClient & client,
										   const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("404", client);
	payload += channel_name + " :Cannot send to channel";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NOORIGIN(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("409", client) + ":No origin specified";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NORECIPIENT(const IRCClient & client, const std::string & command) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("411", client);
	payload += ":No recipient given (" + command + ")";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NOTEXTTOSEND(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("412", client) + ":No text to send";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NICKNAMEINUSE(const IRCClient & client, const std::string & nickname) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("433", client);
	payload += nickname + " :Nickname is already in use";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NICKCOLLISION(const IRCClient & client,
										const IRCClient & collided_client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("436", client);
	payload += collided_client.get_nickname() + " :Nickname collision KILL from ";
	payload += collided_client.get_username() + "@" + collided_client.get_hostname();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_USERNOTINCHANNEL(const IRCClient & client, const std::string & channel_name,
											const std::string & target) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("441", client);
	payload += target + " " + channel_name + " :They aren't on that channel";
	return TCPMessage(receivers, payload);	
}

TCPMessage make_reply_ERR_NOTONCHANNEL(const IRCClient & client, const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("442", client);
	payload += channel_name + " :You're not on that channel";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_ALREADYREGISTRED(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("462", client);
	payload += ":Unauthorized command (already registered)";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_PASSWDMISMATCH(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("464", client) + ":Password incorrect";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_KEYSET(const IRCClient & client, const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("467", client);
	payload += ":" + channel_name + " :Channel key already set";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_UNKNOWNMODE(const IRCClient & client, const std::string & channel_name, const char & mode) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("472", client);
	payload.push_back(mode);
	payload += " :is unknown mode char to me for " + channel_name;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_CHANOPRIVSNEEDED(const IRCClient & client,
										   const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("482", client);
	payload += ":" + channel_name + " :You're not channel operator";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_UMODEUNKNOWNFLAG(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("501", client) + ":Unknown MODE flag";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_USERSDONTMATCH(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = prepare_reply_RPL_ERR("502", client);
	payload += ":Cannot change mode for other users";
	return TCPMessage(receivers, payload);
}
