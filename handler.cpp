#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 6

void pass( std::list<std::string>* args, client& c ) {
	(void) args;
	(void) c;
	logger( "DEBUG", "PASS COMMAND" );
}

void user( std::list<std::string>* args, client& c ) {
	(void) args;
	(void) c;
	logger( "DEBUG", "USER COMMAND" );
}

void privmsg( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "INFO", "%s wants to send a message", c.nick.c_str() );
}

void join( std::list<std::string>* args, client& c ) {
	(void) args;
	logger( "INFO", "%s wants to join", c.nick.c_str() );
}

void nick( std::list<std::string>* args, client& c ) {
	(void) args;
	if ( args->empty() ) {
		c.reply( "431\r\n" );
	} else {
		c.nick         = args->front();
		c.isregistered = true;
		c.reply( "001\r\n" );
		logger( "INFO", "client %d nickname is %s", c.fd, c.nick.c_str() );
	}
}

void capls( std::list<std::string>* args, client& c ) {
	(void) c;
	(void) args;
}

//Function Quit to close the connexion between the client and server

//Funtion Privmsg send a message to client (from client) / or to a channel (many client)

//

void handler( std::list<std::string>* args, client& c ) {
	std::string commands[COMMAND_COUNT] = { "PASS", "USER", "NICK", "JOIN",
	                                        "CAPLS" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, client & c ) = {
	    &pass, &user, &nick, &join, &privmsg, &capls };
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
