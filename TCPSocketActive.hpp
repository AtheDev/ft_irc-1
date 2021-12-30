#ifndef TCPSERVERACTIVE_HPP
# define TCPSERVERACTIVE_HPP

#include "TCPSocket.hpp"

class TCPSocketActive : public TCPSocket
{
	public:

		TCPSocketActive(int socketfd);
		~TCPSocketActive();

		void        start();
		void        close_fd();
		std::string receive_data();
		void        send_data(std::string s);

        int         get_socket_fd(void) const;

	protected:

	private:

};
#endif