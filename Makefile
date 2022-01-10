NAME_SERVER = ircserv

SRCS_SERVER =	main.cpp \
				IRCMessage.cpp
#main.cpp \
#				IRCServer.cpp \
#				TCPServer.cpp \
#				TCPClient.cpp \
#				TCPSocket.cpp \
#				TCPSocketActive.cpp \
#				TCPSocketPassive.cpp \
#				TCPMessage.cpp \
#				IRCMessage.cpp \
#				IRCClient.cpp


OBJS_SERVER = $(SRCS_SERVER:.cpp=.o)

CXX = clang++

CXXFLAGS = -std=c++98 #-Wall -Wextra -Werror

RM = rm -rf

all: $(NAME_SERVER)

$(NAME_SERVER): $(OBJS_SERVER)
				$(CXX) $(CXXFLAGS) $(OBJS_SERVER) -o $(NAME_SERVER)

$(OBJS_SERVER): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $< -g

clean:
	$(RM) $(OBJS_SERVER)

fclean: clean
	$(RM) $(NAME_SERVER)

re: fclean all

.PHONY: all clean fclean re
