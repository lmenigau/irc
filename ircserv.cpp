#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "handler.hpp"
#include "ircserv.hpp"
#include "ostream.hpp"
#include "parsing.hpp"
#include "utils.hpp"

#define MAX_PORT 65535

int                            ircserv::_port   = 0;
bool                           ircserv::_failed = false;
std::string                    ircserv::_password;
client                         ircserv::_clients[1024];
int                            ircserv::_pollfd;
int                            ircserv::_tcp6_socket;
std::map<std::string, channel> ircserv::_channels;

void ircserv::initialisation( char* pass, char* port ) {
	if ( strlen( port ) > 5 ) {
		_failed = true;
		logger( "ERROR", "port value too high !" );
		return;
	}
	_port = atoi( port );
	if ( _port > MAX_PORT || _port <= 0 ) {
		_failed = true;
		logger( "ERROR", "incorrect port value !" );
		return;
	}
	if ( strlen( pass ) == 0 ) {
		_failed = true;
		logger( "ERROR", "password cannot be empty !" );
		return;
	}
	_password = pass;
}

bool ircserv::failed( void ) {
	return _failed;
}

void ircserv::accept_client( epoll_event& ev ) {
	sockaddr_in6 addr = {};
	// socklen_t addrlen = sizeof(sockaddr_in6);
	(void) ev;
	int fd = accept( _tcp6_socket, NULL, 0 );
	logger( "INFO", "%d %d", fd, addr.sin6_port );
	if ( fd >= 0 ) {
		_clients[fd].setFd( fd );
		_clients[fd].start = 0;
		epoll_event event  = { EPOLLIN, { .ptr = &_clients[fd] } };
		epoll_ctl( _pollfd, EPOLL_CTL_ADD, fd, &event );
		_clients[fd].buf.reserve( 512 );
	} else
		logger( "ERROR", "accept error" );
}

void ircserv::process_events( epoll_event& ev ) {
	// std::cout << ev;
	char    buf[512];
	client* c;
	size_t  len;
	if ( ev.events & EPOLLIN ) {
		if ( ev.data.fd == _tcp6_socket ) {
			accept_client( ev );
		} else {
			c   = (client*) ev.data.ptr;
			len = read( c->getFd(), buf, 512 );
			if ( len == 0 ) {
				close( c->getFd() );
				logger( "DEBUG", "closed : %d", c->getFd() );
				return;
			}
			c->buf.append( buf, len );
			for ( ;; ) {
				size_t pos = c->buf.find( "\n" );
				if ( pos == std::string::npos )
					break;
				std::list<std::string>* args = parse( c->buf.substr( 0, pos ) );
				handler( args, *c );
				c->buf.erase( 0, pos + 1 );
			}
		}
	} else if ( ev.events & EPOLLOUT ) {
		c   = (client*) ev.data.ptr;
		len = c->out.copy( buf, 512 );
		len = write( c->getFd(), buf, len );
		c->out.erase( 0, len );
		if ( c->out.empty() ) {
			epoll_event event = { EPOLLIN, { c } };
			epoll_ctl( _pollfd, EPOLL_CTL_MOD, c->getFd(), &event );
		}
	} else {
		std::cerr << ev;
	}
}

void ircserv::start( void ) {
	_tcp6_socket             = socket( AF_INET6, SOCK_STREAM, 0 );
	int                 a    = 1;
	struct sockaddr_in6 addr = { AF_INET6, 0, 0, {}, 0 };
	addr.sin6_port           = htons( _port );
	addr.sin6_addr           = in6addr_any;
	setsockopt( _tcp6_socket, SOL_SOCKET, SO_REUSEADDR, &a, sizeof( a ) );
	int ret = bind( _tcp6_socket, (sockaddr*) &addr, sizeof( addr ) );
	if ( ret < 0 ) {
		std::perror( "ircserv" );
		return;
	}
	listen( _tcp6_socket, 256 );
	// sockaddr_in6 peer_addr = {};
	// socklen_t len = sizeof(peer_addr);
	_pollfd           = epoll_create( 1 );
	epoll_event event = { EPOLLIN, { .fd = _tcp6_socket } };
	epoll_ctl( _pollfd, EPOLL_CTL_ADD, _tcp6_socket, &event );
	logger( "INFO", "server started successfuly" );
	for ( ;; ) {
		epoll_event events[64];
		int         nev = epoll_wait( _pollfd, events, 64, -1 );
		logger( "DEBUG", "nev: %d", nev );
		for ( int i = 0; i < nev; i++ ) {
			process_events( events[i] );
		}
	}
}

int ircserv::getPollfd( void ) {
	return _pollfd;
}

std::string ircserv::getPassword( void ) {
	return _password;
}

std::map<std::string, channel>& ircserv::getChannels( void ) {
	return _channels;
}

void ircserv::addChannel( std::string name ) {
	if ( _channels.find( name ) != _channels.end() )
		return;
	_channels.insert( std::make_pair( name, channel( name ) ) );
}

void ircserv::removeChannel( std::string name ) {
	std::map<std::string, channel>::iterator it = _channels.find( name );
	if ( it == _channels.end() )
		return;
	_channels.erase( it );
}
