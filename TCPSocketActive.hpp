#ifndef TCPSERVERACTIVE_HPP
# define TCPSERVERACTIVE_HPP

#include "TCPSocket.hpp"
class TCPSocketActive : public TCPSocket
{
	public:

		TCPSocketActive(int socketfd);
		~TCPSocketActive();
		void start();
		void close_fd();
		std::string receive();
		void send_data(std::string s);

        int     get_socket_fd(void) const;
        void    set_socket_fd(int fd);

	protected:

	private:

};
#endif