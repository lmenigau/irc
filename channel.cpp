#include "channel.hpp"
#include "utils.hpp"
#include "client.hpp"

std::map<std::string, Client> _clients;

Channel::Channel( void ){}
Channel::~Channel( void ){}

Channel::Channel( std::string name ) : _name( name ){}

Channel::Channel(Client &creator, const std::string &name) : _name(name), _admin(creator)
{
	_clients.insert(std::make_pair(creator.getUser(), creator));
}

void Channel::addClient( Client client ) {
	_clients.insert( std::make_pair(client.getUser(), client ));
}

void Channel::removeClient( Client rclient ) {
	std::map<std::string, Client>::iterator it = _clients.begin();
	while ( it != _clients.end() ) {
		if ( !it->second.getUser().compare( rclient.getUser() ) ) {
			_clients.erase( it );
			return;
		}
		it++;
	}
	logger( "WARNING", "user %s not found in channel %s !",
	        rclient.getUser().c_str(), _name.c_str() );
}

void Channel::changeModes( int n_mode ) {
	(void) n_mode;
	(void) _modes;
	return;
}

std::map<std::string, Client>& Channel::getClients( void ) {
	return _clients;
}

void Channel::setModes( std::string modes ) {
	_modes = modes;
}

std::string Channel::getModes( void ) {
	return _modes;
}
