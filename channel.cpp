#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

Channel::~Channel() {}
Channel::Channel( void ) : _topic_op( false ), _limit( 0 ) {}

Channel::Channel( std::string name )
    : _name( name ), _modes( "" ), _topic_op( false ), _limit( 0 ) {}

Channel::Channel( Client& creator, const std::string& name )
    : _name( name ), _modes( "" ), _topic_op( false ), _limit( 0 ) {
	_ops.push_back( &creator );
	_clients.push_back( &creator );
	_invite_only = false;
}

void Channel::addClient( Client& client ) {
	_clients.push_back( &client );
}

Channel::Channel( const Channel& a )
    : _clients( a._clients ),
      _name( a._name ),
      _modes( a._modes ),
      _topic( a._topic ),
      _password( a._password ),
      _ops( a._ops ),
      _banned( a._banned ),
      _key( a._key ),
      _invite_only( a._invite_only ),
      _topic_op( a._topic_op ),
      _limit( a._limit ) {}

void Channel::removeClient( Client& rclient ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	MessageBuilder                mb;

	while ( it != _clients.end() ) {
		if ( ( *it )->getNick() == rclient.getNick() ) {
			_clients.erase( it );
			return;
		}
		it++;
	}
	logger( "ERROR", mb << "user " << rclient.getUser()
	                    << " not found in channel " << _name << " !" );
}

void Channel::removeClient( Client& rclient, std::string msg ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	MessageBuilder                mb;

	while ( it != _clients.end() ) {
		if ( ( *it )->getNick() == rclient.getNick() ) {
			_clients.erase( it );
			sendAll( mb << ':' << rclient.getNick() << '!' << rclient.getUser()
			            << '@' << rclient.getHostname() << " PART " << _name
			            << " :" << msg << "\r\n",
			         rclient );
			return;
		}
		it++;
	}
	logger( "ERROR", mb << "user " << rclient.getUser()
	                    << " not found in channel " << _name << " !" );
}

void Channel::changeModes( int n_mode ) {
	(void) n_mode;
	(void) _modes;
	return;
}

t_vector_client_ptr& Channel::getClients( void ) {
	return _clients;
}

void Channel::setModes( std::string modes ) {
	_modes = modes;
}

std::string Channel::getModes( void ) {
	return _modes;
}

std::string Channel::addModes( std::string modes ) {
	int j;

	j = 0;
	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( _modes.find( modes[j] ) != std::string::npos )
			continue;
		_modes.insert( _modes.end(), modes[j] );
		j = i;
	}
	return ( _modes );
}

std::string Channel::removeModes( std::string modes ) {
	size_t it;

	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( ( it = _modes.find( modes[i] ) ) == std::string::npos )
			continue;
		_modes.erase( it );
	}
	return ( _modes );
}

void Channel::sendAll( MessageBuilder& mb ) {
	sendAll( mb.getBuff() );
	mb.clear();
}

void Channel::sendAll( std::string msg ) {
	// std::cout << "clients : " << _clients <<
	t_vector_client_ptr::iterator it = _clients.begin();
	for ( ; it != _clients.end(); it++ ) {
		if ( !isBanned( *it ) )
			( *it )->reply( msg );
	}
}

void Channel::sendAll( MessageBuilder& mb, Client& c ) {
	sendAll( mb.getBuff(), c );
	mb.clear();
}

void Channel::sendAll( std::string msg, Client& c ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	// std::cout << "clients : " << _clients << "\n";
	for ( ; it != _clients.end(); it++ ) {
		if ( ( *it )->getFd() != c.getFd() ) {
			( *it )->reply( msg );
		}
	}
}

bool Channel::findClients( const std::string& nick ) {
	t_vector_client_ptr::iterator it = this->_clients.begin();
	// std::cout << "clients : " << _clients << "\n";
	for ( ; it != this->_clients.end(); it++ ) {
		if ( ( *it )->getNick() == nick )
			return ( true );
	}
	return ( false );
}

void Channel::reply_334( Client& c ) {
	MessageBuilder mb;

	mb << ":" << ircserv::getServername() << " 324 " << c.getNick() << " "
	   << _name << " +" << _modes;
	if ( !getKey().empty() )
		mb << " " << getKey();
	if ( _limit )
		mb << " " << _limit;
	mb << "\r\n";
	c.reply( mb );
}

void Channel::handleModes( Client& c, std::string modes, std::string args ) {
	t_ope          operation;
	MessageBuilder mb;

	if ( !isOps( c ) ) {
		c.reply( mb << ":" << ircserv::getServername() << " 482 " << c.getNick()
		            << " :You're not channel operator\r\n" );
		return;
	}

	if ( modes.size() >= 2 && modes[0] == '+' && modes[1] == 'b' &&
	     args.empty() )
		operation = NONE;
	else if ( modes[0] == '+' )
		operation = ADD;
	else if ( modes[0] == '-' )
		operation = SUB;
	else
		operation = NONE;

	for ( std::string::iterator it = modes.begin(); it != modes.end(); it++ ) {
		if ( *it == '-' || *it == '+' )
			continue;
		switch ( *it ) {
			case 'k':
				m_key( c, args, operation );
				break;
			case 'i':
				m_invite( c, args, operation );
				break;
			case 'o':
				m_operator( c, args, operation );
				break;
			case 't':
				m_topic( c, args, operation );
				break;
			case 'b':
				m_ban( c, args, operation );
				break;
			case 'l':
				m_limit( c, args, operation );
				break;
			default:
				// DEBUG
				break;
		}
	}
}
