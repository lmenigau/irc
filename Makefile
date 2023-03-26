CXX=clang++
CXXFLAGS=-std=c++98 -g #-Wall -Werror -Wextra
SRC = handler.cpp\
		ircserv.cpp\
		main.cpp\
		ostream.cpp\
		parsing.cpp
OBJ = $(SRC:.cpp=.o)
NAME = ircserv

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
.PHONY: all clean fclean re
