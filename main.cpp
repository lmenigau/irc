#include "handler.hpp"
#include "irc.hpp"
#include "ircserv.hpp"

int main( int ac, char** av ) {
	if ( ac != 3 ) {
		std::cout << "usage : ./ircserv <port> <password>" << std::endl;
		return ( 1 );
	}
	ircserv::initialisation( av[2], av[1] );
	if ( ircserv::failed() ) {
		std::cout << "FATAL : initialisation has failed !\n";
		return ( 1 );
	}
	ircserv::start();
}
