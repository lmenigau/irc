#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

t_vector_client_ptr& Channel::getOps( void ) {
	return ( _ops );
}

bool Channel::isOps( Client& c ) {
	for ( t_vector_client_ptr::iterator it = _ops.begin(); it != _ops.end();
	      it++ ) {
		if ( c.getFd() == ( *it )->getFd() )
			return ( true );
	}
	return ( false );
}


void Channel::m_operator( Client& c, std::string args, t_ope operation ) {
	std::string    target;
	MessageBuilder mb;
	Client*        client;
	size_t         i = 0;

	while (i != args.size())
	{
		target = getTarget( i, args );
		if ( target.empty() )
			continue;
		if (!findClients( target )) {
			c.reply( mb << ":" << ircserv::getServername() << " 441 "
			            << c.getNick() << " " << target
			            << " :They aren't on that channel\r\n" );
			continue;
		}
		client = find_client( target );
		t_vector_client_ptr::iterator it =
		    std::find( _ops.begin(), _ops.end(), client );
		if ( operation == ADD ) {
			this->sendAll( mb << ":" << c.getNick() << "~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			                  << " +o " << target << "\r\n" );
			if ( it == _ops.end() )
				_ops.push_back( client );
			addModes("o");
		} else {
			if ( it == _ops.end() ) {
				c.reply( mb << ":" << ircserv::getServername() << " 441 "
				            << target << " " << _name
				            << " :They aren't not op on that channel\r\n" );
				continue;
			}
			_ops.erase( it );
			removeModes("o");
			this->sendAll( mb << ":" << c.getNick() << "~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			                  << " -o " << target << "\r\n" );
		}
	}
}
