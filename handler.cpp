#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 9

void nick( std::list<std::string>* args, Client &c);
void user( std::list<std::string>* args, Client& c );

void pass( std::list<std::string>* args, Client& c ) {
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

void privmsg( std::list<std::string>* args, Client& c ) {
	(void) args;
	logger( "INFO", "%s wants to send a message", c.getNick().c_str() );
}

void join( std::list<std::string>* args, Client& c ) {
	std::map<std::string, Channel>           channels = ircserv::getChannels();
	std::map<std::string, Channel>::iterator it;
	args->front().erase( args->front().length() - 1, 1 );
	logger( "INFO", "%s joined channel %s", c.getNick().c_str(),
	        args->front().c_str() );
	it = channels.find( args->front() );
	if ( it == channels.end() ) {
		ircserv::addChannel( args->front(), c );
		it = ircserv::getChannels().find( args->front() );
	}
	it->second.addClient( c );
	c.reply( ":royal!foo.example.bar JOIN #test\r\n" );
	c.reply( ":ircserv.localhost 353 royal = #test :@royal\r\n" );
	c.reply( ":ircserv.localhost 366 royal #test :End of NAMES list\r\n" );
	// c.reply( format( ":ircserv.localhost 332 :%s :no topic\r\n",
	//                 args->front().c_str() ) );
	// c.reply( format( ":ircserv.localhost 353 : :\r\n" ) );
}

void capls( std::list<std::string>* args, Client& c ) {
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

void pong( std::list<std::string>* args, Client& c ) {
	(void) args;
	//args->front().erase( args->back().length() - 1, 1 );
	//logger( "DEBUG", "PING from %s, token = %s", c.getNick().c_str(),
	     //   args->back().c_str()m );
	c.reply( format( "PONG\r\n") );
}

void mode( std::list<std::string>* args, Client& c ) {
	if (args->empty())
		return ;
	args->back().erase( args->back().length() - 1, 1 );
	logger( "DEBUG", "user %s has now mode %s", c.getUser().c_str(),
	        args->back().c_str() );
	c.reply( format( ":ircserv.localhost 221 %s %s\r\n", c.getUser().c_str(),
	                 args->back().c_str() ) );
}

void handler( std::list<std::string>* args, Client& c ) {
	std::string commands[COMMAND_COUNT] = { "PASS", "USER",    "NICK",
	                                        "JOIN", "PRIVMSG", "CAPLS",
	                                        "CAP",  "PING",    "MODE" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, Client & c ) = {
	    &pass, &user, &nick, &join, &privmsg, &capls, &capls, &pong, &mode };
	for ( size_t i = 0; i < COMMAND_COUNT; i++ ) {
	//	std::cout << args->front() << std::endl;
		if ( !args->front().compare( commands[i] ) ) {
			args->pop_front();
			handlers[i]( args, c );
			return;
		}
	}
	logger( "WARNING", "%s COMMAND DO NOT EXIST (YET?)",
	        args->front().c_str() );
}
