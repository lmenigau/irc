#include "channel.hpp"
#include "utils.hpp"

channel::channel( void ){};
channel::~channel( void ){};

channel::channel( std::string name ) : _name( name ){};

void channel::addClient( client client ) {
	_clients.push_back( client );
}

void channel::removeClient( client rclient ) {
	std::vector<client>::iterator it = _clients.begin();
	while ( it != _clients.end() ) {
		if ( !it->getUser().compare( rclient.getUser() ) ) {
			_clients.erase( it );
			return;
		}
		it++;
	}
	logger( "WARNING", "user %s not found in channel %s !",
	        rclient.getUser().c_str(), _name.c_str() );
}

void channel::changeModes( int n_mode ) {
	(void) n_mode;
	(void) _modes;
	return;
}

std::vector<client>& channel::getClients( void ) {
	return _clients;
}
