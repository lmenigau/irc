#include "client.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#include "ircserv.hpp"
#include "utils.hpp"

Client::Client() {
	_fd               = -1;
	start             = 0;
	end               = 0;
	buf               = std::string();
	out               = std::string();
	_cap              = std::string();
	_nick             = std::string();
	_hostname         = std::string();
	_realuser         = std::string();
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isPolled         = false;
	_hasBeenWelcomed  = false;
	_server_op        = false;
	_isBot            = false;
	_invisible        = false;
	_destroy          = false;
	buf.reserve( 512 );
}

Client::Client( Client const& a ) {
	buf.reserve( 512 );
	_fd               = a._fd;
	start             = a.start;
	end               = a.end;
	buf               = std::string().append( a.buf );
	out               = std::string().append( a.out );
	_cap              = std::string().append( a._cap );
	_nick             = std::string().append( a._nick );
	_hostname         = std::string().append( a._hostname );
	_realuser         = std::string().append( a._realuser );
	_hasGivenNick     = a._hasGivenNick;
	_hasGivenUser     = a._hasGivenUser;
	_hasGivenPassword = a._hasGivenPassword;
	_isPolled         = a._isPolled;
	_hasBeenWelcomed  = a._hasBeenWelcomed;
	_server_op        = a._server_op;
	_isBot            = a._isBot;
	_invisible        = a._invisible;
	_destroy          = a._destroy;
}

void Client::reply( std::string const& str ) {
	if ( !_isPolled ) {
		epoll_event event = { EPOLLOUT | EPOLLIN, { .ptr = this } };
		epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_MOD, _fd, &event );
	}
	out += str;
}

void Client::reply( MessageBuilder& mb ) {
	if ( !_isPolled ) {
		epoll_event event = { EPOLLOUT | EPOLLIN, { .ptr = this } };
		epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_MOD, _fd, &event );
	}
	out += mb.getBuff();
	mb.clear();
}

Client::Client( int fd ) {
	_fd               = fd;
	start             = 0;
	end               = 0;
	buf               = std::string();
	out               = std::string();
	_cap              = std::string();
	_nick             = std::string();
	_hostname         = std::string();
	_realuser         = std::string();
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isPolled         = false;
	_hasBeenWelcomed  = false;
	_server_op        = false;
	_isBot            = false;
	_invisible        = false;
	_destroy          = false;
	buf.reserve( 512 );
}

Client::Client( int fd, sockaddr_in6& addr ) {
	setHostname( addr );
	_fd               = fd;
	start             = 0;
	end               = 0;
	buf               = std::string();
	out               = std::string();
	_cap              = std::string();
	_nick             = std::string();
	_realuser         = std::string();
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isPolled         = false;
	_hasBeenWelcomed  = false;
	_server_op        = false;
	_isBot            = false;
	_invisible        = false;
	_destroy          = false;
	buf.reserve( 512 );
}

Client::~Client( void ) {
}


int Client::getFd( void ) {
	return _fd;
}

std::string Client::getCap( void ) {
	return _cap;
}

//getters

std::string Client::getNick( void ) {
	return _nick;
}

std::string Client::getUser( void ) {
	return _user;
}

std::string Client::getModes( void ) {
	return _modes;
}

std::string Client::getRealUser( void ) {
	return _realuser;
}

std::string Client::getHostname( void ) {
	return _hostname;
}
// setters

void Client::setCap( std::string cap ) {
	_cap = cap;
}

void Client::setNick( std::string nick ) {
	_nick = nick;
}

void Client::setIsPolled( bool f ) {
	_isPolled = f;
}

void Client::setHasGivenNick( bool f ) {
	_hasGivenNick = f;
}

void Client::setHasGivenUser( bool f ) {
	_hasGivenUser = f;
}

void Client::setHasGivenPassword( bool f ) {
	_hasGivenPassword = f;
}

void Client::setFd( int fd ) {
	_fd = fd;
}

bool Client::isServerOp( void ) {
	return ( _server_op );
}

void Client::setOp( void ) {
	_server_op = true;
}

void Client::setInvisible( void ) {
	_invisible = true;
}

void Client::setBot( void ) {
	_isBot = true;
}

void Client::unsetInvisible( void ) {
	_invisible = false;
}

void Client::unsetBot( void ) {
	_isBot = false;
}

void Client::unsetOp( void ) {
	_server_op = false;
}

void Client::setHasBeenWelcomed( bool f ) {
	_hasBeenWelcomed = f;
}

void Client::setHostname( sockaddr_in6& addr ) {
	char str_addr[256];
	char hostname[256];

	_hostname = "localhost";
	if ( !inet_ntop( AF_INET6, &addr.sin6_addr, str_addr, 256 ) )
		logger( "ERROR", strerror( errno ) );
	int ret = getnameinfo( (struct sockaddr*) &addr, sizeof( addr ), hostname,
	                       256, NULL, 0, 0 );
	if ( ret != 0 ) {
		logger( "ERROR", strerror( errno ) );
		return;
	}
	_hostname = hostname;
}

void Client::setUser( std::string user ) {
	_user = user;
}

void Client::setModes( std::string modes ) {
	_modes = modes;
}

void Client::setRealUser( std::string realuser ) {
	_realuser = realuser;
}

// state accessorts ?

bool Client::isPolled( void ) {
	return _isPolled;
}

bool Client::hasGivenNick( void ) {
	return _hasGivenNick;
}

bool Client::hasGivenUser( void ) {
	return _hasGivenUser;
}

bool Client::hasGivenPassword( void ) {
	return _hasGivenPassword;
}

bool Client::isRegistered( void ) {
	return ( _hasGivenNick && _hasGivenUser && _hasGivenPassword );
}

bool Client::hasBeenWelcomed( void ) {
	return _hasBeenWelcomed;
}

std::string Client::addModes( std::string modes ) {
	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( _modes.find( modes[i] ) != std::string::npos )
			continue;
		_modes.append( 1, modes[i] );
		if ( modes[i] == 'B' )
			setBot();
		else if ( modes[i] == 'i' )
			setInvisible();
		else if ( modes[i] == 'o' )
			setOp();
	}
	return ( _modes );
}

std::string Client::removeModes( std::string modes ) {
	size_t it;

	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( ( it = _modes.find( modes[i] ) ) == std::string::npos )
			continue;
		_modes.erase( it );
	}
	return ( _modes );
}

void Client::setIsOper( bool b ) {
	_isOper = b;
}

bool Client::isOper( void ) {
	return _isOper;
}

void Client::setDestroy( void ) {
	_destroy = true;
}

bool Client::toDestroy( void ) {
	return _destroy;
}
