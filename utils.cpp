#include "utils.hpp"
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include "channel.hpp"
#include "client.hpp"
#include "ircserv.hpp"

void remove_backslash_r( std::string& c ) {
	size_t idx = c.find( '\r' );
	if ( idx != std::string::npos )
		c.erase( idx, 1 );
}

std::string trim( const std::string& s ) {
	std::string res;

	res = s;
	std::remove( res.begin(), res.end(), ' ' );
	return ( res );
}

std::string format( std::string format, ... ) {
	va_list va_args;
	size_t  i = 0;

	va_start( va_args, format );
	std::string s( format );
	while ( s.find( '%', i ) != std::string::npos ) {
		i = s.find( '%', i );
		if ( s[i + 1] == 's' )
			s.replace( i, 2, (char*) va_arg( va_args, char* ) );
		else if ( s[i + 1] == 'c' ) {
			s.replace( i, 1, 1, ' ' );
			s.replace( i + 1, 1, 1, (char) va_arg( va_args, int ) );
		} else if ( s[i + 1] == 'd' ) {
			std::stringstream ss;
			ss << (int) va_arg( va_args, int );
			s.replace( i, 2, ss.str() );
		} else
			break;
	}
	va_end( va_args );
	return ( s );
}

std::string format( std::string format, va_list va_args ) {
	size_t i = 0;

	std::string s = format;
	while ( s.find( '%', i ) != std::string::npos ) {
		i = s.find( '%', i );
		if ( s[i + 1] == 's' )
			s.replace( i, 2, (char*) va_arg( va_args, char* ) );
		else if ( s[i + 1] == 'c' ) {
			s.replace( i, 1, 1, ' ' );
			s.replace( i + 1, 1, 1, (char) va_arg( va_args, int ) );
		} else if ( s[i + 1] == 'd' ) {
			std::stringstream ss;
			ss << (int) va_arg( va_args, int );
			s.replace( i, 2, ss.str() );
		} else
			break;
	}
	return ( s );
}

bool isUser( std::string str ) {
	return ( str.find( '!' ) != std::string::npos ||
	         str.find( '@' ) != std::string::npos || std::isalpha( str[0] ) );
}

bool isChannel( std::string str ) {
	if ( str[0] == '#' )
		return ( true );
	return ( false );
}

bool hasMode( Client& user, char mode ) {
	return ( user.getModes().find( mode ) != std::string::npos );
}

bool hasMode( Channel& user, char mode ) {
	return ( user.getModes().find( mode ) != std::string::npos );
}

Client* find_client( std::string nick ) {
	for ( t_client_array::iterator it = ircserv::getClients().begin();
	      it != ircserv::getClients().end(); it++ ) {
		if ( it->getNick() == nick )
			return ( &( *it ) );
	}
	return ( NULL );
}

Channel* find_channel( std::string name ) {
	try {
		return ( &ircserv::getChannels().at( name ) );
	} catch ( std::out_of_range& e ) {
		return ( NULL );
	}
}

void welcome( Client* client ) {
	client->reply(
	    format( ":ircserv.localhost 001 %s :Welcome to the FT_IRC "
	            "server %s[!%s@%s]\r\n",
	            client->getNick().c_str(), client->getNick().c_str(),
	            client->getUser().c_str(), client->getHostname().c_str() ) );
	client->reply(
	    format( ":ircserv.localhost 002 %s :Your host is FT_IRC running "
	            "version 0.0.1dev\r\n",
	            client->getNick().c_str() ) );
	client->reply(
	    format( ":ircserv.localhost 003 %s :This server was created idk "
	            "like now ?\r\n",
	            client->getNick().c_str() ) );
	client->reply(
	    format( ":ircserv.localhost 004 %s :FT_IRC 0.0.1dev ia i\r\n",
	            client->getNick().c_str() ) );
	client->setHasBeenWelcomed( true );
}

void close_client( Client& client ) {
	client.reply( ":ircserv.localhost QUIT :Connection closed\r\n" );
	close( client.getFd() );
}
std::string getTarget( size_t& pos, std::string str ) {
	std::size_t pos_comma;
	std::string res;

	pos_comma = str.find( ",", pos );
	if ( pos_comma != std::string::npos ) {
		res = str.substr( pos, pos_comma - pos );
		pos = pos_comma + 1;
	} else {
		res = str.substr( pos );
		pos = str.size();
	}
	return ( res );
}

bool isValidPositiveNumber( std::string args ) {
	long long buff;

	for ( std::string::iterator it = args.begin(); it != args.end(); it++ ) {
		if ( !( std::isdigit( *it ) ) )
			return ( false );
	}
	buff = std::atol( args.c_str() );
	if ( buff < INT_MIN || buff > INT_MAX || buff < 0 )
		return ( false );
	return ( true );
}
