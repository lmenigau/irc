#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

// From my point of view only OPs or Creator should be able to change the topic
// of a channel.
void topic( std::list<std::string>* args, Client& c ) {
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it;

	if ( !args->size() )
		return (
		    c.reply( format( ":%s!%s 461 : Not enough parameters\r\n",
		                     c.getNick().c_str(), c.getHostname().c_str() ) ) );
	if ( args->size() == 1 ) {
		it = channels.find( args->front() );
		if ( it != channels.end() ) {
			std::cout << "TOPIC :: "
			          << ircserv::getChannels()
			                 .find( args->front() )
			                 ->second.getTopic()
			          << std::endl;
			if ( it->second.topicSet() ) {
				c.reply( format( ":ircserv.localhost 332 %s %s :%s\r\n",
				                 c.getNick().c_str(), args->front().c_str(),
				                 it->second.getTopic().c_str() ) );
				// Maybe we do the 333 whotime ..
			} else
				return ( c.reply(
				    format( ":ircserv.localhost 331 %s %s :No topic is set\r\n",
				            c.getNick().c_str(), args->front().c_str() ) ) );
		} else
			return ( c.reply(
			    format( ":ircserv.localhost 403 %s :No such channel\r\n",
			            args->front().c_str() ) ) );
	} else {
		it = channels.find( args->front() );
		if ( it != channels.end() ) {
			if ( it->second.findClients( c.getNick() ) ) {
				if ( it->second.isOps( c ) || !it->second.getInviteMode() ) {
					// Should sennd to all clients the new topic !
					ircserv::getChannels()
					    .find( args->front() )
					    ->second.setTopic( args->back() );
					//					std::cout << "TOPIC :: " <<
					//ircserv::getChannels().find(args->front())->second.getTopic()
					//<< std::endl;
					return ( it->second.sendAll( format(
					    ":%s~!u@%s TOPIC %s :%s\r\n", c.getNick().c_str(),
					    c.getHostname().c_str(), args->front().c_str(),
					    args->back().c_str() ) ) );
				}
			} else
				return (
				    c.reply( format( ":ircserv.localhost 482 %s :You're not "
				                     "channel operator\r\n",
				                     args->front().c_str() ) ) );
		} else
			return ( c.reply(
			    format( ":ircserv.localhost 403 %s :No such channel\r\n",
			            args->front().c_str() ) ) );
		// set the topic
	}
}
