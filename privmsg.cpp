#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void privmsg( std::list<std::string>* args, Client& c ) {
	if ( args->empty() )
		return;
	// void *buf = (const_cast<char *>(args->back().c_str()));
	std::string target = args->front();
	if ( isUser( target ) ) {
		Client* receiver = find_client( target );
		if ( receiver ) {
			receiver->reply(
			    format( ":%s!~%s PRIVMSG %s :%s\r\n", c.getNick().c_str(),
			            ( c.getUser() + "@" + c.getHostname() ).c_str(),
			            target.c_str(), args->back().c_str() ) );
			c.reply( format( ":%s!~%s PRIVMSG %s :%s\r\n", c.getNick().c_str(),
			                 ( c.getUser() + "@" + c.getHostname() ).c_str(),
			                 target.c_str(), args->back().c_str() ) );
		} else {
			c.reply( format( ":%s 401 %s %s :No such nick/channel\r\n",
			                 ircserv::getServername().c_str(),
			                 c.getNick().c_str(), target.c_str() ) );
		}
	} else if ( isChannel( target ) ) {
		Channel* channel = find_channel( target );
		if ( channel ) {
			channel->sendAll(
			    format( ":%s!%s PRIVMSG %s :%s\r\n", c.getNick().c_str(),
			            ( c.getUser() + "@" + c.getHostname() ).c_str(),
			            target.c_str(), args->back().c_str() ),
			    c );
		} else {
			c.reply( format( ":%s 401 %s %s :No such nick/channel\r\n",
			                 ircserv::getServername().c_str(),
			                 c.getNick().c_str(), target.c_str() ) );
		}
	}
	logger( "INFO", "%s wants to send a message", c.getNick().c_str() );
}
