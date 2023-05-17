#include <list>
#include "channel.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

void part( std::list<std::string>* args, Client& c ) {
	std::string    target = args->front();
	MessageBuilder mb;
	Channel*       channel;

	if ( !isChannel( target ) ) {
		c.reply( mb << ':' << ircserv::getServername() << " 403 " << target
		            << " :No such channel\r\n" );
		return;
	}
	channel = find_channel( target );
	if ( !channel ) {
		c.reply( mb << ':' << ircserv::getServername() << " 403 " << target
		            << " :No such channel\r\n" );
		return;
	}
	if ( !channel->findClients( c.getNick() ) ) {
		c.reply( mb << ':' << ircserv::getServername() << " 442 " << target
		            << " :You're not on that channel\r\n" );
		return;
	}
	if ( args->size() == 1 )
		channel->sendAll( mb << ":" << c.getNick() << "!~" << c.getHostname()
		                     << " PART " << target << "\r\n" );
	else
		channel->sendAll( mb << ":" << c.getNick() << "!~" << c.getHostname()
		                     << " PART " << target << " :" << args->back()
		                     << "\r\n" );
	channel->removeClient( c );
}
