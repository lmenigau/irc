#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void	privmsg_client(std::list <std::string > * args, Client &c)
{
	for ( t_map_client::iterator it = ircserv::getClients().begin();
	      									it != ircserv::getClients().end(); it++ ) {
		if ( it->second.getNick() == args->front() ) {
			args->front().append( "@" + ( it->second.getHostname() ) );
			it->second.reply(
			    format( ":%s!~%s PRIVMSG %s: %s\r\n", c.getNick().c_str(),
			            ( c.getUser() + "@" + c.getHostname() ).c_str(),
			            args->front().c_str(), args->back().c_str() ) );
			c.reply( format( ":%s!~%s PRIVMSG %s :%s\r\n", c.getNick().c_str(),
			                 ( c.getUser() + "@" + c.getHostname() ).c_str(),
			                 args->front().c_str(), args->back().c_str() ) );
		} else {
			c.reply( format( ":%s 401 %s %s :No such nick\r\n",
			                 ircserv::getServername().c_str(),
			                 c.getNick().c_str(), args->front().c_str() ) );
		}
	}
}

void	privmsg_channel(std::list <std::string> * args, Client &c)
{
	t_map_channel           channels = ircserv::getChannels();

	Channel* channel = find_channel( args->front() );
	if ( channel ) {
		channel->sendAll(
		    format( ":%s!%s PRIVMSG %s :%s\r\n", c.getNick().c_str(),
		            ( c.getUser() + "@" + c.getHostname() ).c_str(),
		            args->front().c_str(), args->back().c_str() ),
		    		c );
		} else {
			c.reply( format ("%s!~%s@%s 403 %s :No such channel\r\n", c.getNick().c_str(),
															c.getUser().c_str(),
															c.getHostname().c_str(),
															args->front().c_str()));
	}
}

void privmsg( std::list<std::string>* args, Client &c) {
	if ( args->empty() )
		return;
	std::string target = args->front();
	//std::cout << target << std::endl;
		while (args->size() != 1)
		{
		//	std::cout << "args :" << args->front() << std::endl;
			if (isChannel(target))
				privmsg_channel(args, c);
			else
				privmsg_client(args, c);
			args->pop_front();
		}


	// void *buf = (const_cast<char *>(args->back().c_str()));

	//logger( "INFO", "%s wants to send a message", c.getNick().c_str() );
}
