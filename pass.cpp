#include <list>
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"

void pass( std::list<std::string>* args, Client& c ) {
	MessageBuilder mb;
	if ( c.hasGivenPassword() ) {
		c.reply( mb << ':' << ircserv::getServername()
		          << " 462 " << c.getNick() << " :Unauthorized command (already registered)\r\n" );
		return;
	}
	if ( args->front().compare( ircserv::getPassword() ) != 0 ) {
		logger( "ERROR", "client %d : wrong password (%s)!", c.getFd(),
		        args->front().c_str() );
		close( c.getFd() );
		return;
	}
	c.setHasGivenPassword( true );
}
