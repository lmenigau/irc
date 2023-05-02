#include <list>
#include "utils.hpp"
#include "ircserv.hpp"

void pass( std::list<std::string>* args, Client& c ) {
	if ( c.hasGivenPassword() ) {
		c.reply(
		    format( ":%s 462 %s :Unauthorized command (already registered)",
		            ircserv::getServername().c_str(), c.getNick().c_str() ) );
		return ;
	}
	if ( args->front().compare( ircserv::getPassword() ) != 0 ) {
		logger( "ERROR", "client %d : wrong password (%s)!", c.getFd(),
		        args->front().c_str() );
		close( c.getFd() );
		return;
	}
	c.setHasGivenPassword( true );
}
