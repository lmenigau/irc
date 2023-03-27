#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"

#define COMMAND_COUNT 6

void pass( std::list<std::string>* args, client& c ) {
	(void) args;
	std::cout << "PASS COMMAND\n";
}

void user( std::list<std::string>* args, client& c ) {
	(void) args;
	std::cout << "USER COMMAND\n";
}

void privmsg( std::list<std::string>* args, client& c ) {
	std::cout << c.nick << "wants to send a message \n";
}

void join( std::list<std::string>* args, client& c ) {
	std::cout << c.nick << "wants to join \n";
}

void nick( std::list<std::string>* args, client& c ) {
	(void) args;
	if ( args->empty() ) {
		c.reply( "431\r\n" );
	} else {
		c.nick         = args->front();
		c.isregistered = true;
		c.reply( "001\r\n" );
		std::cout << "nickname is " << c.nick << "\n";
	}
}

void capls( std::list<std::string>* args, client& c ) {
	(void) c;
	(void) args;
}

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
	std::cout << args->front() << "COMMAND DO NOT EXIST (YET?)\n";
}
