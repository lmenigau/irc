#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

static void privmsg_client( std::list<std::string>* args, Client& c ) {
	Client* target = find_client( args->front() );
	if ( target ) {
		args->front();
		target->reply( format( ":%s!~%s PRIVMSG %s: %s\r\n",
		                       c.getNick().c_str(),
		                       ( c.getUser() + "@" + c.getHostname() ).c_str(),
		                       args->front().c_str(), args->back().c_str() ) );
	} else {
		c.reply( format( ":%s 401 %s %s :No such nick\r\n",
		                 ircserv::getServername().c_str(), c.getNick().c_str(),
		                 args->front().c_str() ) );
	}
}

static void privmsg_channel( std::list<std::string>* args, Client& c ) {
	t_map_channel channels = ircserv::getChannels();

	Channel* channel = find_channel( args->front() );
	if ( !channel->findClients( c.getNick() ) || channel->isBanned( &c ) )
		return ( c.reply(
		    format( ":ircserv.localhost 404 %s :Cannot send to channel\r\n",
		            args->front().c_str() ) ) );
	if ( channel ) {
		channel->sendAll(
		    format( ":%s!%s PRIVMSG %s :%s\r\n", c.getNick().c_str(),
		            ( c.getUser() + "@" + c.getHostname() ).c_str(),
		            args->front().c_str(), args->back().c_str() ),
		    c );
	} else {
		c.reply( format( "%s!~%s@%s 403 %s :No such channel\r\n",
		                 c.getNick().c_str(), c.getUser().c_str(),
		                 c.getHostname().c_str(), args->front().c_str() ) );
	}
}

void privmsg( std::list<std::string>* args, Client& c ) {
	if ( args->empty() )
		return;
	std::string target = args->front();
	// std::cout << target << std::endl;
	while ( args->size() != 1 ) {
		//	std::cout << "args :" << args->front() << std::endl;
		if ( isChannel( target ) )
			privmsg_channel( args, c );
		else
			privmsg_client( args, c );
		args->pop_front();
	}

	// void *buf = (const_cast<char *>(args->back().c_str()));

	// logger( "INFO", "%s wants to send a message", c.getNick().c_str() );
}