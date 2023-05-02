#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ostream.hpp"
#include "utils.hpp"
#include "typedef.hpp"
#include <iostream>

 Channel::~Channel() {}
 Channel::Channel( void ) {}

Channel::Channel( std::string name ) : _name( name ) {}

Channel::Channel( Client& creator, const std::string& name ) : _name( name ) {
	_ops.push_back( &creator );
	_clients.push_back( &creator );
}

//Is it working ?
void Channel::addClient( Client &client ) {
	_clients.push_back( &client );
}

Channel::Channel(const Channel &a) : _clients(a._clients), _modes(a._modes), _name(a._name), 
									_topic(a._topic), _password(a._password), _ops(a._ops), _halfops(a._halfops), _voiced(a._voiced), _founder(a._founder), _banned(a._banned), _key(a._key)
{}									

void Channel::removeClient( Client& rclient ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	while ( it != _clients.end() ) {
		if ( ( *it )->getFd() == rclient.getFd() ) {
			_clients.erase( it );
			return;
		}
		it++;
	}
	logger( "ERROR", "user %s not found in channel %s !",
	        rclient.getUser().c_str(), _name.c_str() );
}

void Channel::changeModes( int n_mode ) {
	(void) n_mode;
	(void) _modes;
	return;
}

bool	Channel::isOps(Client &c)
{
	for (t_vector_client_ptr::iterator it = _ops.begin(); it != _ops.end(); it++)
	{
		if (c.getFd() ==  ( *it)->getFd())
				return (true);
	}
	return (false);
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
	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( _modes.find( modes[i] ) != std::string::npos )
			continue ;
		_modes.insert( modes.end(), modes[i] );
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

void Channel::sendAll( std::string msg ) {
	//std::cout << "clients : " << _clients <<
	t_vector_client_ptr::iterator it = _clients.begin();
	for ( ; it != _clients.end(); it++ ) {
		( *it )->reply( msg );
	}
}

t_vector_client_ptr &Channel::getOps(void)
{
	return (_ops);
}

void Channel::sendAll( std::string msg, Client& c ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	//std::cout << "clients : " << _clients << "\n";
	for ( ; it != _clients.end(); it++ ) {
		if ( ( *it )->getFd() != c.getFd() ) {
			( *it )->reply( msg );
		}
	}
}
