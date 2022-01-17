#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <ostream>

#define USER_MODES() ("iroO")

class Channel {

	public:
		explicit Channel(std::string & name);
		virtual ~Channel();

		std::vector<int> get_clients() const;
		const std::string & get_name() const;
		const std::string & get_topic() const;
		std::vector<int> get_channel_op() const;

		void set_name(const std::string & name);
		void set_topic(const std::string & topic);

		bool add_client(int socketfd);
		bool add_client_to_channel_operator(int socketfd);
		bool remove_client(int socketfd);

		std::vector<int>::const_iterator clients_begin() const;
		std::vector<int>::const_iterator clients_end() const;
		std::vector<int>::iterator channel_op_begin();
		std::vector<int>::iterator channel_op_end();

		friend std::ostream & operator<<(std::ostream & os, const Channel & channel);

	private:
		std::string _name; // The channel's name
		std::vector<int> _clients; // The channel's clients
		//int _channel_creator; // The channel's creator
		std::vector<int> _channel_operators; // The channel's operators
		//std::vector<int> _banned_clients; // The channel ban list

		//2811::2.3: Channels with '+' as prefix do not support channel modes !
		std::string	_topic;

};

std::ostream & operator<<(std::ostream & os, const Channel & channel);

#endif //CHANNEL_HPP
