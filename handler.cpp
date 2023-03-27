#include "client.hpp"
#include "irc.hpp"

#define COMMAND_COUNT 5

void _pass( std::list<std::string>* args, client& c ) {
	(void) args;
	(void) c;
	std::cout << "PASS COMMAND\n";
}

void _user( std::list<std::string>* args, client& c ) {
	(void) args;
	(void) c;
	std::cout << "USER COMMAND\n";
}

void _join( std::list<std::string>* args, client& c ) {
	(void) args;
	(void) c;
	std::cout << c.nick << "wants to join \n";
}
void _nick( std::list<std::string>* args, client& c ) {
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

void _capls( std::list<std::string>* args, client& c ) {
	(void) c;
	(void) args;
}

void handler( std::list<std::string>* args, client& c ) {
	std::string commands[COMMAND_COUNT] = { "PASS", "USER", "NICK", "JOIN",
	                                        "CAPLS" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, client& ) = {
	    &_pass, &_user, &_nick, &_join, &_capls };
	for ( size_t i = 0; i < COMMAND_COUNT; i++ ) {
		if ( !args->front().compare( commands[i] ) ) {
			args->pop_front();
			handlers[i]( args, c );
			return;
		}
	}
	std::cout << args->front() << "COMMAND DO NOT EXIST (YET?)\n";
}
