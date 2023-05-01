#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ostream.hpp"
#include "utils.hpp"
#include "typedef.hpp"
#include <iostream>

Channel::Channel( void ) {}
Channel::~Channel( void ) {}

Channel::Channel( std::string name ) : _name( name ) {}

Channel::Channel( Client& creator, const std::string& name ) : _name( name ) {
	_ops.push_back( &creator );
	_clients.insert( std::make_pair( creator.getNick(), &creator ) );
}

void Channel::addClient( Client &client ) {
	std::cout << "map before :" << _clients.size() << std::endl;
	_clients.insert( std::make_pair( client.getNick(), &client ) );
	std::cout << "map cient size :" << _clients.size() << std::endl;
	for (t_map_string_client_ref::iterator it = _clients.begin(); it != _clients.end(); it++)
	{
		std::cout << "DEBUUUF" <<it->second->getNick() << std::endl;;
	}
}

Channel::Channel(const Channel &a) : _clients(a._clients), _modes(a._modes), _name(a._name), 
									_topic(a._topic), _password(a._password), _ops(a._ops), _halfops(a._halfops), _voiced(a._voiced), _founder(a._founder), _banned(a._banned), _key(a._key)
{}									

void Channel::removeClient( Client& rclient ) {
	t_map_string_client_ref::iterator it = _clients.begin();
	while ( it != _clients.end() ) {
		if ( it->second->getFd() == rclient.getFd() ) {
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

t_map_string_client_ref& Channel::getClients( void ) {
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
	t_map_string_client_ref::iterator it = _clients.begin();
	for ( ; it != _clients.end(); it++ ) {
		it->second->reply( msg );
	}
}

void Channel::sendAll( std::string msg, Client& c ) {
	t_map_string_client_ref::iterator it = _clients.begin();
	//std::cout << "clients : " << _clients << "\n";
	for ( ; it != _clients.end(); it++ ) {
		if ( it->second->getFd() != c.getFd() ) {
			it->second->reply( msg );
		}
	}
}
