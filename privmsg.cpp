#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"

static void privmsg_client( std::list<std::string>* args, Client& c ) {
	Client* target = find_client( args->front() );
	MessageBuilder mb;
	if ( target ) {
		args->front();
		target->reply( mb << ':' << c.getNick() << "!" << c.getUser() << "@"
		                  << c.getHostname() << " PRIVMSG " << args->front()
		                  << " :" << args->back() << "\r\n" );
	} else {
		c.reply( mb << ':' << ircserv::getServername()
		              << " 401 " << c.getNick() << " " << args->front()
		              << " :No such nick\r\n" );
	}
}

static void privmsg_channel( std::list<std::string>* args, Client& c ) {
	t_map_channel channels = ircserv::getChannels();
	MessageBuilder mb;

	Channel* channel = find_channel( args->front() );
	if ( !channel->findClients( c.getNick() ) || channel->isBanned( &c ) )
		return ( c.reply( mb << ':' << ircserv::getServername()
		                      << " 404 " << c.getNick() << " " << args->front()
		                      << " :Cannot send to channel\r\n" ) );
	if ( channel ) {
		channel->sendAll( mb << ':' << c.getNick() << "!" << c.getUser()
		                     << "@" << c.getHostname() << " PRIVMSG "
		                     << args->front() << " :" << args->back() << "\r\n",
		                 c );
	} else {
		c.reply( mb << ':'
		              << ircserv::getServername() << " 403 " << c.getNick()
		              << " " << args->front() << " :No such channel\r\n" );
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