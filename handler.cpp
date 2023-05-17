#include "handler.hpp"
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 16

void capls( std::list<std::string>* args, Client& c ) {
	(void) args;
	c.reply( "CAP * LS :multi-prefix\r\n" );
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
	MessageBuilder mb;
	c.reply( mb << ":" << ircserv::getServername() << " PONG " << args->back()
	            << "\r\n" );
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
	size_t         i = 0;
	MessageBuilder mb;

	std::string commands[4] = { "PASS", "USER", "NICK", "CAP" };
	void ( *handlers[4] )( std::list<std::string>*, Client& c ) = {
	    pass, user, nick_notregistered, capls };
	for ( ; i < 4; i++ ) {
		//	std::cout << args->front() << std::endl;
		if ( !args->front().compare( commands[i] ) ) {
			args->pop_front();
			remove_backslash_r( args->back() );
			handlers[i]( args, c );
			break;
		}
	}
	if ( i == 4 ) {
		c.reply( mb << ":" << ircserv::getServername()
		            << " 451 * :You are not registered\r\n" );
		return;
	}
	if ( c.isRegistered() && !c.hasBeenWelcomed() &&
	     authorize_setting_name( c.getNick(), c ) ) {
		c.reply( mb << c.getUser() << "!" << c.getHostname() << " NICK "
		            << c.getNick() << "\r\n" );
		welcome( &c );
	} else if ( c.isRegistered() ) {
		c.setHasGivenNick( false );
			c.reply(	mb << ":" << ircserv::getServername() << " 433 * " << c.getNick()
		            << " : Nickname is already in use\r\n" );
	}
}

void handler( std::list<std::string>* args, Client& c ) {
	MessageBuilder mb;

	if ( args->size() == 1 ) {
		c.reply( mb <<  ":" << ircserv::getServername() << " 461 " << args->front()
		            << " :Not enough parameters\r\n" );
		return;
	}
	if ( !c.isRegistered() ) {
		not_registered( args, c );
		return;
	}
	std::string commands[COMMAND_COUNT] = {
	    "PASS",  "USER",   "NICK",   "JOIN",  "PRIVMSG", "CAPLS",
	    "CAP",   "PING",   "MODE",   "WHOIS", "QUIT",    "PART",
	    "TOPIC", "NOTICE", "INVITE", "OPER" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, Client& c ) = {
	    &pass, &user,  &nick, &join, &privmsg, &capls,  &capls,  &pong,
	    &mode, &whois, &quit, &part, &topic,   &notice, &invite, &oper };
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
