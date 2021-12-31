#ifndef TCPSOCKET_HPP
# define TCPSOCKET_HPP

# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <string>


class TCPSocket {
	public:

		virtual void start() = 0;
		virtual void close_fd() = 0;
		virtual int get_socket_fd() const = 0;

		class Cexception : public std::exception {
			public:
				virtual const char * what() const throw();
		};

	protected:

		int _socketfd;
		struct sockaddr_in _address;

	private:

};

#endif