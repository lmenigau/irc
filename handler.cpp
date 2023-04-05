#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 9

void pass( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "DEBUG", "PASS COMMAND" );
	args->front().erase( args->front().length() - 1, 1 );
	if ( args->front().compare( ircserv::getPassword() ) != 0 ) {
		logger( "ERROR", "client %d : wrong password (%s)!", c.getFd(),
		        args->front().c_str() );
		close( c.getFd() );
		return;
	}
	c.setHasGivenPassword( true );
}

void user( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "INFO", "client %d has username %s", c.getFd(),
	        args->front().c_str() );
	c.setHasGivenUser( true );
	c.setUser( args->front() );
	c.reply( format(
	    ":ircserv.localhost 001 %s :Welcome to the FT_IRC "
	    "Network, %s[!%s@foo.example.bar]\r\n",
	    c.getUser().c_str(), c.getNick().c_str(), c.getUser().c_str() ) );
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
	args->front().erase( args->front().length() - 1, 1 );
	if ( args->empty() ) {
		c.reply( "431\r\n" );
	} else {
		c.setNick( args->front() );
		c.setHasGivenNick( true );
		logger( "INFO", "client %d nickname is %s", c.getFd(),
		        c.getNick().c_str() );
	}
}

void capls( std::list<std::string>* args, client& c ) {
	(void) c;
	(void) args;
}

std::ostream& operator<<( std::ostream& os, std::list<std::string> arg ) {
	std::list<std::string>::iterator it = arg.begin();
	while ( it != arg.end() ) {
		os << *it;
		it++;
	}
	return os;
}

void pong( std::list<std::string>* args, client& c ) {
	args->front().erase( args->back().length() - 1, 1 );
	logger( "DEBUG", "PING from %s, token = %s", c.getNick().c_str(),
	        args->back().c_str() );
	c.reply( format( "PONG %s\r\n", args->back().c_str() ) );
	std::cout << *args << "\n";
}

void mode( std::list<std::string>* args, client& c ) {
	args->back().erase( args->back().length() - 1, 1 );
	logger( "DEBUG", "user %s has now mode %s", c.getUser().c_str(),
	        args->back().c_str() );
	c.reply( format( "MODE :%s %s\r\n", c.getUser().c_str(),
	                 args->back().c_str() ) );
}

void handler( std::list<std::string>* args, client& c ) {
	std::string commands[COMMAND_COUNT] = { "PASS", "USER",    "NICK",
	                                        "JOIN", "PRIVMSG", "CAPLS",
	                                        "CAP",  "PING",    "MODE" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, client & c ) = {
	    &pass, &user, &nick, &join, &privmsg, &capls, &capls, &pong, &mode };
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
