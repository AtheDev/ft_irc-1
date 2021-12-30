#ifndef TCPSOCKET_HPP
# define TCPSOCKET_HPP

# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string>

class TCPSocket
{
	public:

		virtual void start() = 0;
		virtual void close() = 0;
		class Cexception : public std::exception
		{
			public:
				virtual const char* what() const throw();
		};

	protected:

		int					_socketfd;
		struct sockaddr_in	_address;

	private:

};

class TCPSocketPassive : public TCPSocket
{
	public:

		TCPSocketPassive(int port);
		~TCPSocketPassive();
		void start();
		void close();
		int accept_connection();

	protected:

	private:

		struct addrinfo	_address_info;

};

class TCPSocketActive : public TCPSocket
{
	public:

		TCPSocketActive(int socketfd);
		~TCPSocketActive();
		void start();
		void close();
		std::string receive_data();
		void send_data(std::string s);

	protected:

	private:

};

#endif