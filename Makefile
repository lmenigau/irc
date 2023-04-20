$(if $(filter re as, $(MAKECMDGOALS)),\
	$(shell $(RM) arte/*.o ft std))
CXX=clang++
CXXFLAGS=-std=c++98 -Wall -Werror -Wextra -g
SRC = channel.cpp\
	  	client.cpp\
	  	handler.cpp\
		ircserv.cpp\
		logger.cpp\
		main.cpp\
		nick.cpp \
		ostream.cpp\
		parsing.cpp\
		privmsg.cpp \
		quit.cpp \
		user.cpp \
		utils.cpp \
		whois.cpp
OBJ = $(addprefix arte/, $(SRC:.cpp=.o))
NAME = ircserv

DEPS := $(addprefix arte/, $(SRC:.cpp=.d))
all: $(NAME)

$(OBJ) $(DEPS): | arte

arte:
	mkdir -p arte

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ)

$(DEPS): arte/%.d: %.cpp
	@set -e; rm -f $@; \
		$(CXX) -MM $(CPPFLAGS) $< > $@.$$$$; \
		sed 's,\($*\)\.o[ :]*, arte/\1.o $@ : ,g' < $@.$$$$ > $@; \
		rm -f $@.$$$$

include $(DEPS)

clean:
	rm -rf arte

fclean: clean
	rm -f $(NAME)

re: all

$(OBJ): arte/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
.PHONY: all clean fclean re
