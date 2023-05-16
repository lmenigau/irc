#include <csignal>
#include "ircserv.hpp"
#include "utils.hpp"

void interupt_handler( int signal ) {
	if ( signal != SIGINT )
		return;

	logger( "WARNING", "INTERRUPT DETECTED" );
	ircserv::stop();
	exit( 0 );
}
