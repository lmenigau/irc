#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"


static void	notice_client(std::list <std::string > * args, Client &c)
{
	for ( t_map_client::iterator it = ircserv::getClients().begin();
	      									it != ircserv::getClients().end(); it++ ) {
		if ( it->second.getNick() == args->front() ) {
			args->front().append( "@" + ( it->second.getHostname() ) );
			it->second.reply(
			    format( ":%s!~%s NOTICE %s: %s\r\n", c.getNick().c_str(),
			            ( c.getUser() + "@" + c.getHostname() ).c_str(),
			            args->front().c_str(), args->back().c_str() ) );
		//	c.reply( format( ":%s!~%s NOTICE %s :%s\r\n", c.getNick().c_str(),
			//                 ( c.getUser() + "@" + c.getHostname() ).c_str(),
			  //               args->front().c_str(), args->back().c_str() ) );
		}
	}
}

void	notice_channel(std::list <std::string> * args, Client &c)
{
	t_map_channel           channels = ircserv::getChannels();

	Channel* channel = find_channel( args->front() );
	if ( channel )
		channel->sendAll(
		    format( ":%s!%s NOTICE %s :%s\r\n", c.getNick().c_str(),
		            ( c.getUser() + "@" + c.getHostname() ).c_str(),
		            args->front().c_str(), args->back().c_str() ),
		    		c );
}

void notice(std::list <std::string> *args, Client &c) {
	if ( args->empty() )
		return;
	while (args->size() != 1)
	{
		if (isChannel(args->front()))
			notice_channel(args, c);
		else
			notice_client(args, c);
		args->pop_front();
	}

}
