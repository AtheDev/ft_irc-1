NAME_SERVER = ircserv

SRCS_SERVER =	main.cpp \
				IRCServer.cpp \
				IRCMessage.cpp \
				IRCClient.cpp \
				Channel.cpp \
				Reply.cpp \
				TCPServer.cpp \
				TCPClient.cpp \
				TCPMessage.cpp \
				TCPSocket.cpp \
				TCPSocketActive.cpp \
				TCPSocketPassive.cpp


OBJS_SERVER = $(SRCS_SERVER:.cpp=.o)

CXX = clang++

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

RM = rm -rf

all: $(NAME_SERVER)

$(NAME_SERVER): $(OBJS_SERVER)
				$(CXX) $(CXXFLAGS) $(OBJS_SERVER) -o $(NAME_SERVER)

$(OBJS_SERVER): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJS_SERVER)

fclean: clean
	$(RM) $(NAME_SERVER)

re: fclean all

.PHONY: all clean fclean re