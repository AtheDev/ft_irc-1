#ifndef TCPMESSAGE_HPP
#define TCPMESSAGE_HPP

#include <string>
#include <vector>
#include <ostream>


class TCPMessage {
	public:
		TCPMessage(int sender, std::string & payload);
		TCPMessage(std::vector<int> & receivers, std::string & payload);
		TCPMessage(int sender, std::vector<int> & receivers, std::string & payload);
		TCPMessage(const TCPMessage & other);


		int get_sender() const;
		const std::vector<int> & get_receivers() const;
		const std::string & get_payload() const;
		friend std::ostream & operator<<(std::ostream & os, const TCPMessage & message);

	private:
		int _sender; // Should be 0 if irrelevant.
		std::vector<int> _receivers; // Should be empty if irrelevant.
		std::string _payload;
};

#endif