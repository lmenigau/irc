// Function to check if the nick is already in the db or not
// If it's the case return 433 "<client> <nick> :Nickname is already in use"

// We can define our accepted character list, if not corresping,
// return 432 "<client> <nick> :Erroneus nickname"

#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

static bool authorize_setting_name( const std::string& name ) {
	for ( t_map_int_client::iterator it = ircserv::_clients.second.begin();
	      it != ircserv::_clients.second.end(); it++ ) {
		if (it->second.getNick() == name )
			return ( false );
	}
	return ( true );
}

void nick( std::list<std::string>* args, Client *c) {
	std::string buff;

	for ( std::list<std::string>::iterator it = args->begin();
	      it != args->end(); it++ ) {
		std::cout << "sent :" << *it << std::endl;
	}
	if ( args->empty() || args->front() == "" ) {
		c->reply( format(
		    ":ircserv.localhost 431 *  NICK: Not enough parameters\r\n" ) );
	} else if ( !authorize_setting_name( args->front() ) ) {
		c->reply( format(
		    ":ircserv.localhost 433 * %s: Nickname is already in use\r\n",
		    args->front().c_str() ) );
	} else if ( c->hasGivenNick() ) {
		buff = c->getNick();
		c->setNick( args->front() );
		c->reply( format( "%s!~%s@%s NICK %s\r\n", buff.c_str(),
		                 c->getUser().c_str(), c->getHostname().c_str(),
		                 c->getNick().c_str() ) );
		logger( "INFO", "User %s nickname change to %s.", c->getUser().c_str(),
		        c->getNick().c_str() );
	}
	c->setNick( args->front() );
	c->setHasGivenNick( true );
	c->reply( format( ":ircserv.localhost 001 %s : Welcome to ft_irc %s\r\n",
	                 c->getNick().c_str(), c->getNick().c_str() ) );
	logger( "INFO", "New user %s nickname %s set, Connexion etablished !",
	        c->getUser().c_str(), c->getNick().c_str() );
}
