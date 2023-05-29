#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"

void quit( std::list<std::string>* args, Client& c ) {
	(void) args;
	MessageBuilder mb;
	if ( !c.hasGivenUser() || !c.hasGivenNick() )
		return;
	c.reply( mb << ':' << c.getNick() << "!" << c.getUser() << "@"
	              << c.getHostname() << " QUIT :" << args->front() << "\r\n" );
	c.reply( mb << "ERROR :" <<  " QUIT :" << args->front() << "\r\n" );
	t_map_channel           *channels = &ircserv::getChannels();
	try {

		for ( ;channels; channels++) {
			(*channels).second.removeClient( c, "left the server");
		}
	}
	catch (...) {
		ircserv::removeClient(c);

	}
	//ircserv::removeClient(c);
}
