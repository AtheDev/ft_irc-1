#include "Reply.hpp"
#include "Utils.hpp"

TCPMessage make_reply_JOIN(const IRCClient & client, const Channel & channel) {
	const std::vector<int>& receivers = channel.get_clients();
	std::string payload = ":" + client.get_prefix() + " JOIN " + channel.get_name();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_PART(const IRCClient & client, const Channel & channel,
						   const std::string & part_message) {
	std::vector<int> receivers = channel.get_clients();
	receivers.push_back(client.get_fd());
	std::string payload = ":" + client.get_prefix() + " PART " + channel.get_name();
	if (!part_message.empty()) {
		payload += " :" + part_message;
	} else {
		payload += " :" + client.get_nickname();
	}
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_TOPIC(const IRCClient & client, const Channel & channel) {
	const std::vector<int>& receivers = channel.get_clients();
	std::string payload;
	payload = ":" + client.get_prefix() + " TOPIC " + channel.get_name();
	payload += " :" + channel.get_topic();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_WELCOME(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload;
	payload += "001 :Welcome to the Internet Relay Network ";
	payload += client.get_nickname() + "!";
	payload += client.get_username() + "@";
	payload += client.get_hostname();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_YOURHOST(const IRCClient & client,
								   const std::string & servername, const std::string & version) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "002 :Your host is " + servername + ", running version " + version;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_CREATED(const IRCClient & client, const std::string & date) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "003 :This server was created " + date;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_MYINFO(const IRCClient & client, const std::string & servername,
								 const std::string & version, const std::string & user_modes,
								 const std::string & channel_modes) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "004 :" + servername + " " + version + " ";
	payload += user_modes + " " + channel_modes;
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_LIST(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload;
	payload = "322 :" + channel.get_name() + " ";
	payload += to_string(channel.get_clients().size()) + " :" + channel.get_topic();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_LISTEND(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "323 :End of LIST";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_NOTOPIC(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "331 :" + channel.get_name() + " :No topic is set";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_RPL_TOPIC(const IRCClient & client, const Channel & channel) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "332 :" + channel.get_name() + " :" + channel.get_topic();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NOSUCHCHANNEL(const IRCClient & client,
										const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "403" + channel_name + " :No such channel";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NICKNAMEINUSE(const IRCClient & client, const std::string & nick) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "433" + nick + " :Nickname is already in use";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NICKCOLLISION(const IRCClient & client,
										const IRCClient & collided_client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "436" + collided_client.get_nickname();
	payload += " :Nickname collision KILL from " + collided_client.get_username();
	payload += "@" + collided_client.get_hostname();
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_NOTONCHANNEL(const IRCClient & client, const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "442" + channel_name + " :You're not on that channel";
	return TCPMessage(receivers, payload);
}

TCPMessage make_reply_ERR_ALREADYREGISTRED(const IRCClient & client) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "462 :Unauthorized command (already registered)";
	return TCPMessage(receivers, payload);
}
TCPMessage make_reply_ERR_CHANOPRIVSNEEDED(const IRCClient & client, const std::string & channel_name) {
	std::vector<int> receivers(1u, client.get_fd());
	std::string payload = "482" + channel_name + " :You're not channel operator";
	return TCPMessage(receivers, payload);
}