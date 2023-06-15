#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"

static void notice_client( std::list<std::string>* args, Client& c ) {
	Client* target = find_client( args->front() );
	MessageBuilder mb;

	if ( target ) {
		args->front();
		target->reply( mb << ':' << c.getNick() << "!" << c.getUser() << "@"
		                  << c.getHostname() << " NOTICE " << args->front()
		                  << " :" << args->back() << "\r\n" );
	}
}

void notice_channel( std::list<std::string>* args, Client& c ) {
	t_map_channel channels = ircserv::getChannels();
	MessageBuilder mb;

	Channel* channel = find_channel( args->front() );
	if ( !channel->findClients( c.getNick() ) || channel->isBanned( &c ) )
		return;
	if ( channel )
		channel->sendAll( mb << ':' << c.getNick() << '!' << c.getUser()
		                     << '@' << c.getHostname() << " NOTICE "
		                     << args->front() << " :" << args->back() << "\r\n", c );
}

void notice( std::list<std::string>* args, Client& c ) {
	if ( args->empty() )
		return;
	while ( args->size() != 1 ) {
		if ( isChannel( args->front() ) )
			notice_channel( args, c );
		else
			notice_client( args, c );
		args->pop_front();
	}
}
