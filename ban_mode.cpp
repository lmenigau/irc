#include <iostream>
#include <string>
#include "channel.hpp"
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

bool Channel::isBanned( Client* c ) {
	if ( std::find( _banned.begin(), _banned.end(), c ) != _banned.end() )
		return ( true );
	return ( false );
}

void Channel::reply_ban_list( Client& c ) {
	MessageBuilder mb;

	for ( t_vector_client_ptr::iterator it = _banned.begin();
	      it != _banned.end(); it++ ) {
		mb << ":ircserv.locahost 367 " << c.getNick() << " " << _name;
		mb << " " << ( *it )->getNick() << "!"
		   << "*@*";
		mb << " " << c.getNick() << "!~" << c.getUser() << "@"
		   << c.getHostname() << "\r\n";
		sendAll( mb );
	}
	(void) c;
	sendAll( ":irvserv.localhost 368 :End of channel ban list\r\n" );
}

void Channel::m_ban( Client& c, std::string args, t_ope operation ) {
	if ( operation == NONE )
		return ( reply_ban_list( c ) );
	std::string    target;
	Client*        client;
	MessageBuilder mb;
	size_t         i = 0;

	while ( i != args.size() ) {
		target = getTarget( i, args );
		if ( target.empty() )
			continue;
		client = find_client( target );
		if ( !findClients( target ) ) {
			c.reply( mb << ":" << ircserv::getServername() << " 441 " << target
			            << " " << _name << " :is not on that channel\r\n" );
			continue;
		}
		t_vector_client_ptr::iterator it =
		    std::find( _banned.begin(), _banned.end(), client );
		if ( operation == ADD ) {
			sendAll( mb << ':' << c.getNick() << '!' << c.getUser() << '@'
			            << c.getHostname() << " MODE " << _name << " +b "
			            << target + "!*@*\r\n" );
			if ( it == _banned.end() )
				_banned.push_back( client );
			reply_ban_list( c );
			addModes( "b" );
		} else {
			if ( it == _banned.end() ) {
				c.reply( mb << ":" << ircserv::getServername() << " 441 "
				            << target << " " << _name
				            << " :They aren't banned on that channel\r\n" );
				continue;
			}
			_banned.erase( it );
			removeModes( "b" );
			sendAll( mb << ':' << c.getNick() << '!' << c.getUser() << '@'
			            << c.getHostname() << " MODE " << _name << " -b "
			            << target + "!*@*\r\n" );
			reply_ban_list( c );
		}
	}
}
