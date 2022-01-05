#ifndef TCPMESSAGE_HPP
#define TCPMESSAGE_HPP

#include <string>
#include <vector>
#include <ostream>


class TCPMessage {
	public:
		TCPMessage(int sender, const std::vector<int>& receivers, const std::string payload);

		int get_sender() const;
		std::vector<int> get_receivers() const;
		std::string get_payload() const;
		friend std::ostream & operator<<(std::ostream & os, const TCPMessage & message);

	private:
		const int _sender; // Should be 0 if irrelevant.
		std::vector<int> _receivers; // Should be empty if irrelevant.
		const std::string _payload;
};

#endif