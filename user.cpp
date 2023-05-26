#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

static std::string get_realname( std::list<std::string>* args ) {
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

void user( std::list<std::string>* args, Client& c ) {
	std::string    username;
	MessageBuilder mb;

	if ( args->empty() ) {
		c.reply(
		    mb << ":ircserv.localhost * 461 USER: Not enough parameters\r\n" );
		return;
	} else if ( !c.hasGivenPassword() ) {
		c.reply( mb << ':' << ircserv::getServername()
		            << " 464 :Password Incorrect" );

		logger( "ERROR",
		        mb << "client " << c.getFd() << " did not give password !" );
		close( c.getFd() );
		return;
	} else if ( c.hasGivenUser() ) {
		c.reply( mb << ":ircserv.localhost * 462: You may not reregister\r\n" );
		return;
	} else {
		logger( "INFO", mb << "client " << c.getFd() << " has username "
		                   << args->front() );
		c.setHasGivenUser( true );
		c.setUser( args->front() );
		// Checker la command 0 *: (real name)
		c.setRealUser( get_realname( args ) );
		// c.reply( format("~%s\r\n", c->getUser().c_str()));
	}
}
