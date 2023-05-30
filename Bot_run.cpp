#include "Bot.hpp"


int main(int ac, char **av)
{
	if (ac != 4) {
		//logger( "INFO", "usage : ./ircbot <addr> <port> <password>");
		return ( 1 );
	}
	if (!Bot::connectToServer(av))
		return (2);
	Bot::intro();
	Bot::end();
}
