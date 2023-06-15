#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

void quit( std::list<std::string>* args, Client& c ) {
	(void) args;
	MessageBuilder mb;
	if ( !c.hasGivenUser() || !c.hasGivenNick() )
		return;
	c.reply( mb << ':' << c.getNick() << "!" << c.getUser() << "@"
	            << c.getHostname() << " QUIT :" << args->front() << "\r\n" );
	c.reply( mb << "ERROR :"
	            << " QUIT :" << args->front() << "\r\n" );
	t_map_channel*          channels = &ircserv::getChannels();
	t_map_channel::iterator it       = channels->begin();
	c.end                            = true;
	for ( ; it != channels->end(); it++ ) {
		( *it ).second.removeClient( c, "left the server" );
	}
}
