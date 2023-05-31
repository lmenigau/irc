#include "utils.hpp"
#include <cstdarg>
#include <iostream>
#include <sstream>
#include <string>
#include "channel.hpp"
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"

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
	MessageBuilder mb;

	client->reply( mb << ':' << ircserv::getServername() << " 001 "
	                  << client->getNick()
	                  << " :Welcome to the "
	                     "FT_IRC server "
	                  << client->getNick() << "!" << client->getUser() << "@"
	                  << client->getHostname() << "\r\n" );

	client->reply( mb << ':' << ircserv::getServername() << " 002 "
	                  << client->getNick()
	                  << " Your host is FT_IRC running version 0.0.1dev\r\n" );

	client->reply( mb << ':' << ircserv::getServername() << " 003 "
	                  << client->getNick()
	                  << " This server was created idk like now ?\r\n" );

	client->reply( mb << ':' << ircserv::getServername() << " 004 "
	                  << client->getNick() << " FT_IRC 0.0.1dev ia i\r\n" );

	client->setHasBeenWelcomed( true );
}

void close_client( Client& client ) {
	MessageBuilder mb;
	if ( client.getNick() != "" )
		ircserv::removeClient( client );
	client.reply( mb << ':' << ircserv::getServername()
	                 << "QUIT :Connection closed\r\n" );
	close( client.getFd() );
}

std::string getTarget( size_t& pos, std::string str ) {
	std::size_t pos_comma;
	std::string res;

	if (pos >= str.size() || str.empty())
	{
		pos = str.size();
		return ("");
	}
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

bool check_num(char *port)
{
	int i = 0;

	while (port[i])
	{
		if (port[i] < 48 || port[i] > 57)
			return (false);
		i++;
	}
	return (true);
}
