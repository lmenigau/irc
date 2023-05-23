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
#include "messageBuilder.hpp"

bool authorize_setting_name( const std::string& name, Client& c ) {
	for ( t_client_array::iterator it = ircserv::getClients().begin();
	      it != ircserv::getClients().end(); it++ ) {
		if ( it->getNick() == name && it->getFd() != c.getFd() )
			return ( false );
	}
	return ( true );
}

void nick( std::list<std::string>* args, Client& c ) {
	std::string buff;
	MessageBuilder mb;
	// for ( std::list<std::string>::iterator it = args->begin();
	//       it != args->end(); it++ ) {
	// 	std::cout << "sent :" << *it << std::endl;
	// }
	if ( args->empty() || args->front() == "" ) {
		c.reply( mb << ":" << ircserv::getServername() << " 431 * NICK :Not enough parameters\r\n" );
	} else if ( !authorize_setting_name( args->front(), c ) &&
	            c.isRegistered() ) {
		logger(
		    "INFO",
		    "User %s tried to change nickname to %s but it's already taken.",
		    c.getUser().c_str(), args->front().c_str() );
		c.reply( mb << ":" << ircserv::getServername() << " 433 * "
		             << c.getNick() << " :Nickname is already in use\r\n" );
	} else if ( c.hasGivenNick() ) {
		buff = c.getNick();
		c.setNick( args->front() );
		c.reply( mb << ":" << buff << "!~" << c.getUser() << "@"
		             << c.getHostname() << " NICK " << c.getNick() << "\r\n" );
		logger( "INFO", "User %s nickname change to %s.", c.getUser().c_str(),
		        c.getNick().c_str() );
	} else {
		buff = c.getNick();
		c.setNick( args->front() );
		c.reply( mb << ':' << buff << "!" << c.getUser() << "@"
		             << c.getHostname() << " NICK " << c.getNick() << "\r\n" );
		c.setHasGivenNick( true );
		logger( "INFO", "User %s nickname change to %s.", c.getUser().c_str(),
		        c.getNick().c_str() );
	}
}

void nick_notregistered( std::list<std::string>* args, Client& c ) {
	std::string buff = c.getNick();
	c.setNick( args->front() );
	c.setHasGivenNick( true );
}
