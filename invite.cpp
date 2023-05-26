#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

void invite( std::list<std::string>* args, Client& c ) {
	MessageBuilder mb;

	if ( args->size() == 1 || args->size() == 0 ) {
		c.reply( mb << ":" << ircserv::getServername()
		            << " 461 INVITE :Not enough parameters\r\n" );
		return;
	}
	Channel* channel_target = find_channel( args->back() );
	if ( !channel_target )
		return ( c.reply( mb << ":" << ircserv::getServername() << " 403 "
		                     << args->back() << ":No such channel\r\n" ) );
	if ( !channel_target->findClients( c.getNick() ) )
		return ( c.reply( mb << ":" << ircserv::getServername() << " 442 "
		                     << args->back()
		                     << ":You're not on that channel\r\n" ) );
	while ( args->size() != 1 ) {
		Client* client_target = find_client( args->front() );
		if ( !client_target )
			c.reply( mb << ":" << ircserv::getServername() << " 401 "
			            << args->front() << ":No such nick\r\n" );
		else if ( channel_target->findClients( args->front() ) )
			c.reply( mb << ":" << ircserv::getServername() << " 443 "
			            << args->front() << ' ' << args->back()
			            << " :is already on channel\r\n" );
		else {
			c.reply( mb << ":" << ircserv::getServername() << " 341 "
			            << c.getNick() << args->front() << args->back() );
			client_target->reply( mb << ":" << c.getNick() << "!~"
			                         << c.getHostname() << " INVITE "
			                         << args->front() << " " << args->back()
			                         << "\r\n" );
			channel_target->inviteUser( *client_target );
		}
		args->pop_front();
	}
}
