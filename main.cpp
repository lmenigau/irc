#include "handler.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

int main( int ac, char** av ) {
	if ( ac != 3 ) {
		logger( "INFO", "usage : ./ircserv <port> <password>" );
		return ( 1 );
	}
	ircserv::initialisation( av[2], av[1] );
	if ( ircserv::failed() ) {
		logger( "FATAL", "initialisation has failed !" );
		return ( 1 );
	}
	ircserv::start();
}
