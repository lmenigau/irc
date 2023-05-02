#include "client.hpp"
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include "ircserv.hpp"
#include "utils.hpp"
#include <cerrno>

Client::Client() {
	_fd               = -1;
	start             = 0;
	end               = 0;
	buf               = "";
	out               = "";
	_cap              = "";
	_nick             = "";
	_hostname         = "";
	_realuser         = "";
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isPolled         = false;
	_hasBeenWelcomed  = false;
}

Client::Client(Client const &a)
{ 
	this->_fd 		  = a._fd;
	this->start             = a.start;
	this->end               = a.end;
	this->buf               = a.buf;
	this->out               = a.out;
	this->_cap              = a._cap;
	this->_nick             = a._nick;
	this->_hostname         = a._hostname;
	this->_realuser         = a._realuser;
	this->_hasGivenNick     = a._hasGivenNick;
	this->_hasGivenUser     = a._hasGivenUser;
	this->_hasGivenPassword = a._hasGivenPassword;
	this->_hasBeenWelcomed  = a._hasBeenWelcomed;
	this->_isPolled         = a._isPolled;
}

void Client::reply( std::string const& str ) {
	if ( !_isPolled ) {
		epoll_event event = { EPOLLOUT | EPOLLIN, { .ptr = this } };
		epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_MOD, _fd, &event );
	}
	out += str;
}

Client::Client( int fd ) {
	_fd               = fd;
	start             = 0;
	end               = 0;
	buf               = "";
	out               = "";
	_cap              = "";
	_nick             = "";
	_hostname         = "";
	_realuser         = "";
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isPolled         = false;
	_hasBeenWelcomed  = false;
}

Client::Client( int fd, sockaddr_in6& addr ) {
	setHostname( addr );
	_fd               = fd;
	start             = 0;
	end               = 0;
	buf               = "";
	out               = "";
	_cap              = "";
	_nick             = "";
	_realuser         = "";
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isPolled         = false;
	_hasBeenWelcomed  = false;
}

Client::~Client( void ) {
	/*
	std::map<std::string, Channel> channel_map = ircserv::getChannels();

	for (std::map<std::string, Channel>::iterator it = channel_map.begin(); it != channel_map.end(); it ++)
	{
		std::vector<Client *>::iterator it_chan = std::find(it->second.getClients().begin(), it->second.getClients().end(), this);;
		if (it_chan != it->second.getClients().end())
			it->second.getClients().erase(it_chan);
	}
	ircserv::_clients.erase(std::remove(ircserv::_clients.begin(), ircserv::_clients.end(), this));*/
	for (t_vector_client::iterator it = ircserv::getClients().begin(); it != ircserv::getClients().end(); it++)
	{
		if (this->_fd == it->getFd())
			return ;
	}
	close( _fd );
	logger("DEBUG", "destructor client called");
}

// getters

int Client::getFd( void ) {
	return _fd;
}

std::string Client::getCap( void ) {
	return _cap;
}

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

void Client::setHasBeenWelcomed( bool f ) {
	_hasBeenWelcomed = f;
}

void Client::setHostname( sockaddr_in6& addr ) {
	char str_addr[256];
	char hostname[256];

	_hostname = "";
	if ( !inet_ntop( AF_INET6, &addr.sin6_addr, str_addr, 256 ) )
		logger("ERROR", strerror( errno ) );
	if ( getnameinfo( (struct sockaddr*) &addr, sizeof( addr ), hostname, 256,
	                  NULL, 0, 0 ) != 0 )
		logger("ERROR", strerror( errno ) );
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
