#include "ircserv.hpp"
#include "handler.hpp"
#include "ostream.hpp"
#include "parsing.hpp"
#include "utils.hpp"

#define MAX_PORT 65535

int                 ircserv::_port   = 0;
bool                ircserv::_failed = false;
std::string         ircserv::_password;
t_map_int_client	ircserv::_clients;
std::string	    ircserv::_servername = "ircserv::";
int                 ircserv::_pollfd;
int                 ircserv::_tcp6_socket;
std::map<std::string, Channel> ircserv::_channels;

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
	sockaddr_in6 addr;
	socklen_t    len;
	memset( &addr, 0, sizeof( addr ) );
	len = sizeof( addr );
	// socklen_t addrlen = sizeof(sockaddr_in6);
	(void) ev;
	int fd = accept( _tcp6_socket, (sockaddr*) &addr, &len );
	if ( fd >= 0 ) {
		ircserv::_clients.insert(std::make_pair(fd, Client (fd, addr)));
		Client *ptr = &(ircserv::_clients.find(fd)->second);
		epoll_event event = { EPOLLIN, { .ptr = ptr } };
		epoll_ctl( _pollfd, EPOLL_CTL_ADD, fd, &event );
		ptr->buf.reserve( 512 );
	} else
		logger( "ERROR", "accept error" );
}

void ircserv::process_events( epoll_event& ev ) {
	char    buf[512];
	Client* c;
	size_t  len;
	if ( ev.events & EPOLLIN ) {
		if ( ev.data.fd == _tcp6_socket ) {
			accept_client( ev );
			// std::cout << ircserv::_clients.front().getFd() << std::endl;
		} else {
			c   = reinterpret_cast<Client*>( ev.data.ptr );
			len = read( c->getFd(), buf, 512 );
			std::cout << c->getFd() << std::endl;
			if ( len == 0 ) {
				logger( "INFO", "deleted: %d", c->getFd() );
				ircserv::removeClient( *c );
				return;
			}
			c->buf.append( buf, len );
			for ( ;; ) {
				size_t pos = c->buf.find( "\n" );
				if ( pos == std::string::npos )
					break;
				std::list<std::string>* args = parse( c->buf.substr( 0, pos ) );
				bool	a;
				a = false;
				if (args->front() == "QUIT")
					a = true;
				handler( args, *c );
				if (!a)
					c->buf.erase( 0, pos + 1 );
				else
					break ;
				//logger( "DEBUG", "buf after mdr : %s", c->buf.c_str() );
			}
		}
	} else if ( ev.events & EPOLLOUT ) {
		c   = reinterpret_cast<Client*>( ev.data.ptr );
		len = c->out.copy( buf, 512 );
		logger( "DEBUG", "bruuh buf : %s", c->buf.c_str() );
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
	int b = 0;
	for ( ;; ) {
		epoll_event events[64];
		int         nev = epoll_wait( _pollfd, events, 64, -1 );
		// logger( "DEBUG", "nev: %d", nev );
		for ( int i = 0; i < nev; i++ ) {
			process_events( events[i] );
		}
		b++;
	}
}

int ircserv::getPollfd( void ) {
	return _pollfd;
}

std::string ircserv::getPassword( void ) {
	return _password;
}

t_map_channel& ircserv::getChannels( void ) {
	return _channels;
}

void ircserv::addChannel( std::string& name, Client& client ) {
	if ( _channels.find( name ) != _channels.end() )
		return;
	_channels.insert( std::make_pair( name, Channel( client, name ) ) ); }

void ircserv::removeChannel( std::string name ) {
	t_map_channel::iterator it = _channels.find( name );
	if ( it == _channels.end() )
		return;
	_channels.erase( it );
}

std::string ircserv::getServername( void ) {
	return _servername;
}

void ircserv::removeClient( Client& c ) {
	t_map_int_client::iterator it;
	it = _clients.find(c.getFd());
	if (it != _clients.end()) {
		_clients.erase( it );
	}
}
