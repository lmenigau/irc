#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

#define COMMAND_COUNT 9

void pass( std::list<std::string>* args, Client& c ) {
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

void user( std::list<std::string>* args, Client& c ) {
	if ( !c.hasGivenPassword() ) {
		c.reply( ":ircserv.localhost 464 :Password Incorrect" );
		logger( "WARNING", "client %d did not give password !", c.getFd() );
		close( c.getFd() );
		return;
	}
	logger( "INFO", "client %d has username %s", c.getFd(),
	        args->front().c_str() );
	c.setHasGivenUser( true );
	c.setUser( args->front() );
	c.reply( format(
	    ":ircserv.localhost 001 %s :Welcome to the FT_IRC "
	    "Network, %s[!%s@foo.example.bar]\r\n",
	    c.getUser().c_str(), c.getNick().c_str(), c.getUser().c_str() ) );
	c.reply(
	    format( ":ircserv.localhost 002 %s :Your host is FT_IRC running "
	            "version 0.0.1dev\r\n",
	            c.getUser().c_str() ) );
	c.reply( format(
	    ":ircserv.localhost 003 %s :This server was created idk like now ?\r\n",
	    c.getUser().c_str() ) );
	c.reply( format( ":ircserv.localhost 004 %s :FT_IRC 0.0.1dev ia i\r\n",
	                 c.getUser().c_str() ) );
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
	c.reply(
	    format( ":%s!foo.example.bar JOIN %s\r\n", c.getNick().c_str(), args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 353 %s = %s :@%s\r\n",
	                 c.getUser().c_str(), args->front().c_str(),
	                 c.getNick().c_str() ) );
	c.reply( format( ":ircserv.localhost 366 %s %s :End of NAMES list\r\n",
	                 c.getUser().c_str(), args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 332 :%s :no topic\r\n",
	                 args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 353 : :\r\n" ) );
}

void nick( std::list<std::string>* args, Client& c ) {
	if ( !c.hasGivenPassword() ) {
		c.reply( ":ircserv.localhost 464 :Password Incorrect" );
		logger( "WARNING", "client %d did not give password !", c.getFd() );
		close( c.getFd() );
		return;
	}
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
	args->front().erase( args->back().length() - 1, 1 );
	logger( "DEBUG", "PING from %s, token = %s", c.getNick().c_str(),
	        args->back().c_str() );
	c.reply( format( "PONG %s\r\n", args->back().c_str() ) );
}

/*IRC MODS:
 *
 * +ioRZBT
 *
 * go see documentation on notion !
 *
 */

static bool is_valid_user_mode( char mode ) {
	return ( mode == 'i' || mode == 'o' || mode == 'R' || mode == 'Z' ||
	         mode == 'B' || mode == 'T' );
}

static std::string check_user_modes( std::string modes ) {
	std::string ret;

	if ( modes[0] == '+' )
		ret += '+';
	else if ( modes[0] == '-' )
		ret += '-';
	else
		return ( ret );
	for ( size_t i = 1; i < modes.length(); i++ ) {
		if ( is_valid_user_mode( modes[i] ) ) {
			ret += modes[i];
		}
	}
	return ret;
}

void user_mode( std::list<std::string>* args,
                Client&                 c,
                std::string             target,
                std::string             modes,
                char                    operation ) {
	if ( modes.empty() ) {
		c.reply( format( ":ircserv.localhost 501 %s :Unknown MODE flag\r\n",
		                 c.getNick().c_str() ) );
		return;
	}
	if ( target != c.getNick() ) {
		c.reply( ":ircserv.localhost 502 :Cant change mode for other users" );
		return;
	}
	if ( operation == '+' )
		modes = c.addModes( modes );
	else if ( operation == '-' )
		modes = c.removeModes( modes );
	logger( "DEBUG", "user %s has now mode %s", c.getUser().c_str(),
	        modes.c_str() );
	c.reply( format( ":ircserv.localhost 221 %s %s\r\n", c.getUser().c_str(),
	                 modes.c_str() ) );
}

void channel_mode( std::list<std::string>* args,
                   Client                  c,
                   std::string             target,
                   std::string             modes,
                   char                    operation ) {
	if ( modes.empty() ) {
		c.reply( format( ":ircserv.localhost 324 %s %s +\r\n",
		                 c.getUser().c_str(), target.c_str() ) );
		return;
	}
	logger( "DEBUG", "channel %s has now mode %s", target.c_str(),
	        modes.c_str() );
	c.reply( format( ":ircserv.localhost 324 %s %s +%s\r\n",
	                 c.getUser().c_str(), target.c_str(), modes.c_str() ) );
}
void mode( std::list<std::string>* args, Client& c ) {
	args->back().erase( args->back().length() - 1, 1 );
	std::string modes     = check_user_modes( args->back() );
	char        operation = modes[0];
	modes.erase( 0, 1 );
	std::string target = args->front();
	logger( "DEBUG", "target = %s", target.c_str() );
	if ( isUser( target ) && target != c.getNick() )
		return;
	if ( isUser( target ) && target == c.getNick() ) {
		user_mode( args, c, target, modes, operation );
	}
	if ( isChannel( target ) ) {
		channel_mode( args, c, target, modes, operation );
	}
}

void handler( std::list<std::string>* args, Client& c ) {
	if ( args->size() == 1 ) {
		c.reply( format( ":ircserv.localhost 461 %s :Not enough parameters",
		                 args->front().c_str() ) );
		return;
	}
	std::string commands[COMMAND_COUNT] = { "PASS", "USER",    "NICK",
	                                        "JOIN", "PRIVMSG", "CAPLS",
	                                        "CAP",  "PING",    "MODE" };
	void ( *handlers[COMMAND_COUNT] )( std::list<std::string>*, Client& c ) = {
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
