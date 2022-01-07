#include "TCPSocket.hpp"


/**
 *  @brief Exception class for C function errors that return -1 and edit errno.
 *
 *  @return The raised error's string.
 */
const char * TCPSocket::Cexception::what() const throw() { return strerror(errno); }

/**
 *  @brief Exception class for a EWOULDBLOCK error.
 *
 *  @return The raised error's string.
 */
const char * TCPSocket::WouldBlockException::what() const throw() { return strerror(errno); }
