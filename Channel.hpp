#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <algorithm>
#include <string>
#include <vector>

class Channel {

	public:
		explicit Channel(std::string & name);
		virtual ~Channel();

		void set_name(const std::string & name);
		const std::string & get_name() const;
		void add_client(int socketfd);
		void remove_client(int socketfd);
		std::vector<int>::iterator clients_begin();
		std::vector<int>::iterator clients_end();

	private:
		std::string _name; // The channel's name
		std::vector<int> _clients; // The channel's clients
		//int _channel_creator; // The channel's creator
		//std::vector<int> _channel_operators; // The channel's operators
		//std::vector<int> _banned_clients; // The channel ban list

		//2811::2.3: Channels with '+' as prefix do not support channel modes !

};


#endif //CHANNEL_HPP
