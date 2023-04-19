#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void quit( std::list<std::string>* args, Client& c ) {
	(void) args;
	if ( c.hasGivenUser() && c.hasGivenNick() )
		c.reply( format( "%s!%s@%s QUIT : %s is gone.\r\n", c.getNick().c_str(),
		                 c.getUser().c_str(), c.getHostname().c_str(),
		                 c.getNick().c_str() ) );
}