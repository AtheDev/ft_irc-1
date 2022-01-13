#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <ostream>


class Channel {

	public:
		explicit Channel(std::string & name);
		virtual ~Channel();

		const std::vector<int> & get_clients() const;
		void set_name(const std::string & name);
		const std::string & get_name() const;
		bool add_client(int socketfd);
		bool remove_client(int socketfd);
		std::vector<int>::const_iterator clients_begin() const;
		std::vector<int>::const_iterator clients_end() const;
		friend std::ostream & operator<<(std::ostream & os, const Channel & channel);

	private:
		std::string _name; // The channel's name
		std::vector<int> _clients; // The channel's clients
		//int _channel_creator; // The channel's creator
		//std::vector<int> _channel_operators; // The channel's operators
		//std::vector<int> _banned_clients; // The channel ban list

		//2811::2.3: Channels with '+' as prefix do not support channel modes !

};

std::ostream & operator<<(std::ostream & os, const Channel & channel);

#endif //CHANNEL_HPP
