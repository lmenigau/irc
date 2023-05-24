#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"

// From my point of view only OPs or Creator should be able to change the topic
// of a channel.
void topic( std::list<std::string>* args, Client& c ) {
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it;
	MessageBuilder		  mb;

	if ( !args->size() )
		return (
		    c.reply( mb << ':' << ircserv::getServername()
		                 << " 461 TOPIC :Not enough parameters\r\n" ) );
	if ( args->size() == 1 ) {
		it = channels.find( args->front() );
		if ( it != channels.end() ) {
			std::cout << "TOPIC :: "
			          << ircserv::getChannels()
			                 .find( args->front() )
			                 ->second.getTopic()
			          << std::endl;
			if ( it->second.hasTopic() ) {
				c.reply( mb << ":ircserv.localhost 332 " << c.getNick()
				            << ' ' << args->front() << " :"
				            << it->second.getTopic() << "\r\n" );
				// Maybe we do the 333 whotime ..
			} else
				return ( c.reply( mb << ":ircserv.localhost 331 "
				                     << c.getNick() << ' ' << args->front()
				                     << " :No topic is set\r\n" ) );
		} else
			return ( c.reply( mb << ":ircserv.localhost 403 "
			                     << args->front()
			                     << " :No such channel\r\n" ) );
	} else {
		it = channels.find( args->front() );
		if ( it != channels.end() ) {
			if ( it->second.findClients( c.getNick() ) ) {
				if ( it->second.isOps( c ) || !it->second.topicRight() ) {
					// Should sennd to all clients the new topic !
					ircserv::getChannels()
					    .find( args->front() )
					    ->second.setTopic( args->back() );
					//					std::cout << "TOPIC :: " <<
					//ircserv::getChannels().find(args->front())->second.getTopic()
					//<< std::endl;
					return ( it->second.sendAll( mb << ':' << c.getNick()
					                                 << " TOPIC "
					                                 << args->front()
					                                 << " :" << args->back()
					                                 << "\r\n" ) );
				}
			} else
				return (
				    c.reply( mb << ":ircserv.localhost 442 " << c.getNick()
				                << ' ' << args->front()
				                << " :You're not on that channel\r\n" ) );
		} else
			return ( c.reply( mb << ":ircserv.localhost 403 "
			                     << args->front()
			                     << " :No such channel\r\n" ) );
		// set the topic
	}
}
