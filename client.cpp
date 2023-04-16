#include "client.hpp"
#include <sys/epoll.h>
#include <unistd.h>
#include "ircserv.hpp"
#include "utils.hpp"


Client::Client() {
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
	_isRegistered     = false;
	_isPolled         = false;
}

void Client::reply( std::string const &str ) {
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
	_hasGivenNick     = false;
	_hasGivenUser     = false;
	_hasGivenPassword = false;
	_isRegistered     = false;
	_isPolled         = false;
}

Client::~Client( void ) {}

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

void Client::setUser( std::string user ) {
	_user = user;
}

void Client::setModes( std::string modes ) {
	_modes = modes;
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
