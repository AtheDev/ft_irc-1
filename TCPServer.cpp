#include "TCPServer.hpp"
#include "TCPSocket.hpp"

/**
 *  @brief Buils the TCPServer and initializes its socket.
 *
 *  @param port Port number on which your server will accept incoming connections.
 */
TCPServer::TCPServer(std::string port): _socket(atoi(port.c_str())) {}

/**
 *  @brief Destroys the TCPServer.
 */
TCPServer::~TCPServer() {}

/**
 *  @brief Initialize the pollfd structure of the TCPServer and launch the server.
 */
void    TCPServer::start(void) {

	signal(SIGINT, handler_signal);
	signal(SIGQUIT, handler_signal);

    struct pollfd   new_pollfd;

    new_pollfd.fd = _socket.get_socket_fd();
    new_pollfd.events = POLLIN;
    new_pollfd.revents = 0;
    _pollfds.push_back(new_pollfd);

    std::cout << "\033[0;34m" << "Server opened" << "\033[0m" << std::endl;
    _socket.start();
    _run();
}

/**
 *  @brief Closes all open sockets and frees the memory allocated for TCPClient.
 */
void    TCPServer::stop(void) {

    _pollfds.clear();
    if (!(_clients.empty()))
    {
        std::map<int, TCPClient*>::iterator it = _clients.begin();
        for (; it != _clients.end(); it++)
        {
            it->second->get_socket().close_fd();
            delete  _clients[it->second->get_socket().get_socket_fd()];
        }
        _clients.clear();
    }
    _socket.close_fd();
}

/**
 *  @brief Function that starts an infinite loop to keep the server open
 *  until it receives a closing signal.
 *  Allows to add new TCPClient, to delete a TCPClient when it disconnects
 *	and to resend incoming messages.
 */
void    TCPServer::_run(void) {

    while (1)
    {
        if (poll(&(*_pollfds.begin()), _pollfds.size(), -1) == -1)
            throw ErrorPollException();
        std::vector<struct pollfd>::iterator it = _pollfds.begin();
        std::vector<struct pollfd>::iterator ite = _pollfds.end();
        for (; it != ite; it++)
        {
            if (it->revents == POLLIN)
            {
                if (it->fd == _socket.get_socket_fd())
                    _add_clients();
                else
                {
                    std::map<int, TCPClient*>::iterator it_client = _clients.find(it->fd);
                    std::list<std::string> msg = it_client->second->receive_from();
                    std::list<std::string>::iterator it_list = msg.begin();
                    // ****************** A changer permet de capter une déconnexion *********************
                    std::string q = "quit";
                    if (*it_list == q)
                    {
                        std::cout << "*********** DECONNEXION CLIENT ***************" << std::endl;
                        _remove_client(it->fd);
                        if ((it + 1) == ite)
                            break;
                    }
                    else
                    {
                    // ****************** A changer *********************
                    for (; it_list != msg.end(); it_list++)
                        _send_to_all(*it_list);
                    }
                    msg.clear();
                }
            }
            else if (it->revents == POLLHUP && it->fd != _socket.get_socket_fd())
                std::cout << "*********** DECONNEXION CLIENT ***************" << std::endl;

        }
        it = _pollfds.begin();
        for (; it != _pollfds.end(); it++)
            it->revents = 0;
    }
}

/**
 *  @brief Starts a loop to accept all incoming connections and create new TCPClients.
 */
void    TCPServer::_add_clients(void) {

    int new_fd;
    //try {
        while (true)
        {
            new_fd = _socket.accept_connection();
            if (new_fd < 0)
                break ;
            _add_client(new_fd);
        }
    //}
    /*catch (TCPSocket::Cexception & e) {

        if (errno != EWOULDBLOCK)
            std::cout << e.what() << std::endl;
    }*/
}
/*void    TCPServer::_add_clients(void) {

    int new_fd;
    socklen_t   size_addr;
    struct sockaddr_in  tmp_addr = _socket.get_address();
    do
    {
        new_fd = accept(_socket.get_socket_fd(), (struct sockaddr *)& tmp_addr, &size_addr);
        if (new_fd < 0)
            break ;
        _add_client(new_fd);
    } while (new_fd != -1);
}*/


/**
 *  @brief Creates a new TCPClient and insert it in the map _clients.
 *  Creates and adds the corresponding pollfd structure.
 *
 *  @param socket_fd Corresponds to the fd of the new socket newly created
 *	by the connection acceptance.
 */
void    TCPServer::_add_client(int socket_fd) {

    std::cout << "\033[0;32m" << "Success accept() => New Client" << "\033[0m" << std::endl;

    TCPClient *new_client = new TCPClient(socket_fd);
    _clients.insert(std::pair<int, TCPClient*>(socket_fd, new_client));

    struct pollfd   new_pollfd;

    new_pollfd.fd = socket_fd;
    new_pollfd.events = POLLIN | POLLHUP;
    new_pollfd.revents = 0;
    _pollfds.push_back(new_pollfd);
}

/**
 *  @brief Destroy the TCPClient corresponding to the socket_fd following its disconnection.
 *
 *  @param socket_fd Corresponds to the fd of the TCPClient socket that disconnects
 */
void    TCPServer::_remove_client(int socket_fd) {

    std::vector<struct pollfd>::iterator  it = _pollfds.begin();
    for (; it != _pollfds.end(); it ++)
        if (it->fd == socket_fd)
        {
            _pollfds.erase(it);
            break;
        }
    _clients[socket_fd]->get_socket().close_fd();
    delete  _clients[socket_fd];
    _clients.erase(socket_fd);
}

/**
 *  @brief Sends a message to all TCPClients present.
 *
 *  @param str Corresponds to the message to be sent.
 */
void    TCPServer::_send_to_all(std::string str) {

    std::vector<struct pollfd>::iterator it = _pollfds.begin() + 1;
    for (; it != _pollfds.end(); it++)
        _clients[it->fd]->send_to(str);
}

const char *    TCPServer::ErrorSignalException::what() const throw() {

    return ("\n\033[0;34mServer closed\033[0m");
}

const char *    TCPServer::ErrorPollException::what() const throw() {

    return ("Exception : error poll()");
}
