#include "client.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include "ircserv.hpp"

std::map<std::string, client> users;

void client::reply( std::string const str ) {
	if ( !_ispolled ) {
		epoll_event event = { EPOLLOUT | EPOLLIN, { .ptr = this } };
		epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_MOD, _fd, &event );
	}
	out += str;
}

client::client( int fd ) {
	_fd               = fd;
	start             = 0;
	end               = 0;
	buf               = "";
	out               = "";
	_cap              = "";
	_nick             = "";
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isregistered     = false;
	_ispolled         = false;
}

client::client( void ) {
	_fd               = -1;
	start             = 0;
	end               = 0;
	buf               = "";
	out               = "";
	_cap              = "";
	_nick             = "";
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isregistered     = false;
	_ispolled         = false;
}

client::~client( void ) {
	close( _fd );
}

// getters

int client::getFd( void ) {
	return _fd;
}

std::string client::getCap( void ) {
	return _cap;
}

std::string client::getNick( void ) {
	return _nick;
}

std::string client::getUser( void ) {
	return _user;
}
// setters

void client::setCap( std::string cap ) {
	_cap = cap;
}

void client::setNick( std::string nick ) {
	_nick = nick;
}

void client::setIsPolled( bool f ) {
	_ispolled = f;
}

void client::setHasGivenNick( bool f ) {
	_hasGivenNick = f;
}

void client::setHasGivenUser( bool f ) {
	_hasGivenUser = f;
}

void client::setHasGivenPassword( bool f ) {
	_hasGivenPassword = f;
}

void client::setFd( int fd ) {
	_fd = fd;
}

void client::setUser( std::string user ) {
	_user = user;
}

// state accessorts ?

bool client::isPolled( void ) {
	return _ispolled;
}

bool client::hasGivenNick( void ) {
	return _hasGivenNick;
}

bool client::hasGivenUser( void ) {
	return _hasGivenUser;
}

bool client::hasGivenPassword( void ) {
	return _hasGivenPassword;
}

bool client::isRegistered( void ) {
	return ( _hasGivenNick && _hasGivenUser && _hasGivenPassword );
}
