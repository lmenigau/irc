#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 7

void pass( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "DEBUG", "PASS COMMAND" );
	if ( args->front().compare( ircserv::getPassword() ) ) {
		logger( "ERROR", "client %d : wrong password !", c.getFd() );
		close( c.getFd() );
	}
	c.setHasGivenPassword( true );
}

void user( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "INFO", "client %d has username %s", c.getFd(),
	        args->front().c_str() );
	c.setHasGivenUser( true );
	c.setUser( args->front() );
}

void privmsg( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "INFO", "%s wants to send a message", c.getNick().c_str() );
}

void join( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "INFO", "%s wants to join", c.getNick().c_str() );
}

void nick( std::list<std::string>* args, client& c ) {
	(void) args;
	if ( args->empty() ) {
		c.reply( "431\r\n" );
	} else {
		c.setNick( args->front() );
		c.setHasGivenNick( true );
		c.reply( format(
		    ":ircserv.localhost 001 %s :Welcome to the FT_IRC "
		    "Network, %s[!%s@foo.example.bar]\r\n",
		    c.getUser().c_str(), c.getNick().c_str(), c.getUser().c_str() ) );
		logger( "INFO", "client %d nickname is %s", c.getFd(),
		        c.getNick().c_str() );
	}
}

void capls( std::list<std::string>* args, client& c ) {
	(void) c;
	(void) args;
}

void handler( std::list<std::string>* args, client& c ) {
	std::string commands[COMMAND_COUNT] = { "PASS", "USER",  "NICK",
	                                        "JOIN", "CAPLS", "CAP" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, client& c ) = {
	    &pass, &user, &nick, &join, &privmsg, &capls, &capls };
	for ( size_t i = 0; i < COMMAND_COUNT; i++ ) {
		if ( !args->front().compare( commands[i] ) ) {
			args->pop_front();
			handlers[i]( args, c );
			return;
		}
	}
	logger( "WARNING", "%s COMMAND DO NOT EXIST (YET?)",
	        args->front().c_str() );
}
