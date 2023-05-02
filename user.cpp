#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

// Function Parser

std::string get_realname( std::list<std::string>* args ) {
	std::string res;
	size_t      pos;

	if ( args->empty() )
		return ( "" );
	for ( std::list<std::string>::iterator it = args->begin();
	      it != args->end(); it++ ) {
		pos = it->find( ":" );
		if ( pos != std::string::npos )
			res.append( it->substr( pos + 1 ) );
	}
	return ( trim( res ) );
}

void user( std::list<std::string>* args, Client &c) {
	std::string username;

	if ( args->empty() ) {
		c.reply( format(
		    ":ircserv.localhost * 461 USER: Not enough parameters\r\n" ) );
		return;
	} else if ( !c.hasGivenPassword() ) {
		c.reply( ":ircserv.localhost 464 :Password Incorrect\r\n" );
		logger( "ERROR", "client %d did not give password !", c.getFd() );
		close( c.getFd() );
		return;
	} else if ( c.hasGivenUser() ) {
		c.reply(
		    format( ":ircserv.localhost * 462: You may not reregister\r\n" ) );
		return;
	} else {
		logger( "INFO", "client %d has username %s", c.getFd(),
		        args->front().c_str() );
		c.setHasGivenUser( true );
		c.setUser( args->front() );
		// Checker la command 0 *: (real name)
		c.setRealUser( get_realname( args ) );
		// c.reply( format("~%s\r\n", c->getUser().c_str()));
	}
}
