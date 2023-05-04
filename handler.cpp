#include "handler.hpp"
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 12

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

void pong( std::list<std::string>* args, Client &c) {
	(void) args;
	c.reply( ( format( "PONG %s\r\n", ircserv::getServername().c_str(),
	                   args->back().c_str() ) ) );
}

/*IRC MODS:
 *
 * client : ioRZBT
 * channel : imRMsuU (+ defaults : nt, +specials : bfkl, +roles:qoahv)
 *
 * go see documentation on notion !
 *
 */

void not_registered( std::list<std::string>* args, Client& c ) {
	std::string commands[3] = { "PASS", "USER", "NICK" };
	void ( *handlers[3] )( std::list<std::string>*, Client& c ) = {
	    pass, user, nick_notregistered };
	for ( size_t i = 0; i < 3; i++ ) {
		//	std::cout << args->front() << std::endl;
		if ( !args->front().compare( commands[i] ) ) {
			args->pop_front();
			remove_backslash_r( args->back() );
			handlers[i]( args, c );
			break;
		}
	}
	if ( c.isRegistered() && !c.hasBeenWelcomed() &&
	     authorize_setting_name( c.getNick(), c ) ) {
		c.reply( format( ":%s!%s NICK %s\r\n", c.getUser().c_str(),
		                 c.getHostname().c_str(), c.getNick().c_str() ) );
		welcome( &c );
	} else if ( c.isRegistered() ) {
		c.setHasGivenNick( false );
		c.reply( format(
		    ":ircserv.localhost 433 * %s : Nickname is already in use\r\n",
		    c.getNick().c_str() ) );
	}
}

void handler( std::list<std::string>* args, Client& c ) {
	std::cout << args;
	if ( args->size() == 1 ) {
		c.reply( format( ":ircserv.localhost 461 %s :Not enough parameters\r\n",
		                 args->front().c_str() ) );
		return;
	}
	if ( !c.isRegistered() ) {
		not_registered( args, c );
		return;
	}
	std::string commands[COMMAND_COUNT] = {
	    "PASS", "USER", "NICK", "JOIN",  "PRIVMSG", "CAPLS",
	    "CAP",  "PING", "MODE", "WHOIS", "QUIT",    "PART" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, Client& c ) = {
	    &pass,  &user, &nick, &join,  &privmsg, &capls,
	    &capls, &pong, &mode, &whois, &quit,    &part };
	for ( size_t i = 0; i < COMMAND_COUNT; i++ ) {
		//	std::cout << args->front() << std::endl;
		if ( !args->front().compare( commands[i] ) ) {
			args->pop_front();
			remove_backslash_r( args->back() );
			handlers[i]( args, c );
			return;
		}
	}
	logger( "WARNING", "%s COMMAND DO NOT EXIST (YET?)",
	        args->front().c_str() );
}
