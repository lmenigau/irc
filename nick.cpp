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
	for ( std::vector<Client*>::iterator it = ircserv::_clients.begin();
	      it != ircserv::_clients.end(); it++ ) {
		if ( ( *it )->getNick() == name )
			return ( false );
	}
	return ( true );
}

void nick( std::list<std::string>* args, Client& c ) {
	std::string buff;

	for ( std::list<std::string>::iterator it = args->begin();
	      it != args->end(); it++ ) {
		std::cout << "sent :" << *it << std::endl;
	}
	if ( args->empty() || args->front() == "" ) {
		c.reply( format(
		    ":ircserv.localhost 431 *  NICK: Not enough parameters\r\n" ) );
	} else if ( !authorize_setting_name( args->front() ) ) {
		logger("INFO", "User %s tried to change nickname to %s but it's already taken.\n", c.getUser().c_str(), args->front().c_str());
		c.reply( format(
		    ":ircserv.localhost 433 : Nickname is already in use\r\n",
		    args->front().c_str() ) );
	} else if ( c.hasGivenNick() ) {
		buff = c.getNick();
		c.setNick( args->front() );
		c.reply( format( ":%s!%s NICK %s\r\n", buff.c_str(),
		                 c.getHostname().c_str(), c.getNick().c_str() ) );
		logger( "INFO", "User %s nickname change to %s.\n", c.getUser().c_str(),
		        c.getNick().c_str() );
	} else {
		buff = c.getNick();
		c.setNick( args->front() );
		c.reply( format( ":%s!%s NICK %s\r\n", buff.c_str(),
		                 c.getHostname().c_str(), c.getNick().c_str() ) );
		c.setHasGivenNick( true );
	}
	if ( c.isRegistered() && !c.hasBeenWelcomed())
		welcome(&c);
}
