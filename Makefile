NAME_SERVER = server

SRCS_SERVER = main.cpp TCPServer.cpp TCPSocketPassive.cpp TCPSocketActive.cpp TCPClient.cpp

OBJS_SERVER = $(SRCS_SERVER:.cpp=.o)

CXX = clang++

CXXFLAGS = -Wall -Wextra 

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