#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <algorithm>
#include <string>
#include <vector>
#include <ostream>

#define CHANNEL_MODES() ("ok")
#define CHANNEL_MODES_WITHOUT_PARAMS() ("imnpst")
#define CHANNEL_MODES_WITH_PARAMS() ("blv")

class Channel {

	public:
		explicit Channel(const std::string & name);
		virtual ~Channel();

		const std::vector<int> & get_clients() const;
		const std::string & get_name() const;
		const std::string & get_topic() const;
		std::vector<int> get_channel_op() const;
		const std::string & get_key() const;
		const std::string & get_mode() const;

		void set_name(const std::string & name);
		void set_topic(const std::string & topic);
		void set_key(const std::string & key);
		void set_mode(char sign, char mode);

		bool add_client(int socketfd);
		bool add_client_to_channel_operator(int socketfd);
		bool remove_client(int socketfd);
		bool remove_client_to_channel_operator(int socketfd);

		bool has_client(int socketfd);
		bool is_mode(const char mode);

		std::vector<int>::const_iterator clients_begin() const;
		std::vector<int>::const_iterator clients_end() const;
		std::vector<int>::const_iterator channel_op_begin() const;
		std::vector<int>::const_iterator channel_op_end() const;

		friend std::ostream & operator<<(std::ostream & os, const Channel & channel);

	private:
		std::string _name; // The channel's name
		std::vector<int> _clients; // The channel's clients
		std::vector<int> _channel_operators; // The channel's operators

		std::string	_topic;
		std::string _key;
		std::string _mode;

};

std::ostream & operator<<(std::ostream & os, const Channel & channel);

#endif //CHANNEL_HPP
