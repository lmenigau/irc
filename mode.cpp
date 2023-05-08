#include "utils.hpp"
#include "ircserv.hpp"
#include <map>
#include <list>

#define USERMODE "ioRZBT"
#define CHANNELMODE "imRMsuUntbfklqahv"

static bool is_valid_user_mode( char mode ) {
	return ( mode == 'i' || mode == 'o' || mode == 'R' || mode == 'Z' ||
	         mode == 'B' || mode == 'T' );
}

static bool is_valid_channel_mode( char mode ) {
	return ( mode == 'i' || mode == 'm' || mode == 'R' || mode == 'M' ||
	         mode == 's' || mode == 'u' || mode == 'U' || mode == 'n' ||
	         mode == 't' || mode == 'b' || mode == 'f' || mode == 'k' ||
	         mode == 'l' || mode == 'q' || mode == 'a' || mode == 'h' ||
	         mode == 'v' );
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

static std::string check_channel_modes( std::string modes ) {
	std::string ret;

	if ( modes[0] == '+' )
		ret += '+';
	else if ( modes[0] == '-' )
		ret += '-';
	else
		return ( ret );
	for ( size_t i = 1; i < modes.length(); i++ ) {
		if ( is_valid_channel_mode( modes[i] ) ) {
			ret += modes[i];
		}
	}
	return ret;
}

void user_mode( Client&     c,
                std::string target,
                std::string modes,
                char        operation ) {
	if ( modes.empty() ) {
		c.reply( format( ":ircserv.localhost 501 %s :Unknown MODE flag\r\n",
		                 c.getNick().c_str() ) );
		return;
	}
	if ( target != c.getNick() ) {
		c.reply(
		    ":ircserv.localhost 502 :Cant change mode for other users\r\n" );
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

void channel_mode( Client&     c,
                   std::string target,
                   std::string modes,
                   char        operation ) {
	if ( modes.empty() ) {
		c.reply( format( ":ircserv.localhost 324 %s %s +\r\n",
		                 c.getUser().c_str(), target.c_str() ) );
		return;
	}
	try {
		ircserv::getChannels().at( target );
	} catch ( std::exception& e ) {
		c.reply( format( ":ircserv.localhost 403 %s :No such channel\r\n",
		                 target.c_str() ) );
	}
	logger( "DEBUG", "channel %s has now mode %s", target.c_str(),
	        modes.c_str() );
	c.reply( format( ":ircserv.localhost 324 %s %s +%s\r\n",
	                 c.getUser().c_str(), target.c_str(), modes.c_str() ) );
	(void) operation;
}

void mode( std::list<std::string>* args, Client& c ) {
	if ( args->empty() )
		return;
	std::string modes;
	char        operation = modes[0];
	modes.erase( 0, 1 );
	std::string target = args->front();
	if ( isUser( target ) ) {
		modes = check_user_modes( args->back() );
		user_mode( c, target, modes, operation );
	}
	if ( isChannel( target ) ) {
		modes = check_channel_modes( args->back() );
		channel_mode( c, target, modes, operation );
	}
}
