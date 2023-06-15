$(if $(filter re as, $(MAKECMDGOALS)),\
	$(shell $(RM) arte/*.o ft std))

CXX=clang++
CXXFLAGS=-std=c++98 -Wall -Werror -Wextra -g
SRC =   ban_mode.cpp \
		channel.cpp\
	  	client.cpp\
	  	handler.cpp\
		ircserv.cpp\
		invite.cpp\
		invite_mode.cpp\
		join.cpp\
		key_mode.cpp\
		kick.cpp \
		leave.cpp\
		limit_mode.cpp\
		logger.cpp\
		main.cpp\
		messageBuilder.cpp\
		mode.cpp\
		notice.cpp \
		nick.cpp \
		oper.cpp \
		operator_mode.cpp\
		ostream.cpp\
		parsing.cpp\
		pass.cpp\
		privmsg.cpp \
		quit.cpp \
		signal.cpp \
		topic.cpp \
		topic_mode.cpp\
		user.cpp \
		utils.cpp

OBJ = $(addprefix arte/, $(SRC:.cpp=.o))
NAME = ircserv

DEPS := $(addprefix arte/, $(SRC:.cpp=.d))
all: $(NAME)

$(OBJ) $(DEPS): | arte

arte:
	mkdir -p arte

bonus: ircbot all

ircbot:
	$(CXX) $(CXXFLAGS) bot.cpp Bot_run.cpp logger.cpp messageBuilder.cpp -o ircbot

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
	rm -f ircbot

re: all

debug: all
	./$(NAME) 6667 password

$(OBJ): arte/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
.PHONY: all clean fclean re
