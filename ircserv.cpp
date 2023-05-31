#include "ircserv.hpp"
#include <cerrno>
#include <csignal>
#include "handler.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "parsing.hpp"
#include "signal.hpp"
#include "utils.hpp"

#define MAX_PORT 65535

int            ircserv::_port   = 0;
bool           ircserv::_failed = false;
std::string    ircserv::_password;
t_client_array ircserv::_clients;
std::string   ircserv::_servername = "ircserv.localhost";  //! pls do not change
int           ircserv::_pollfd;
int           ircserv::_tcp6_socket;
t_map_channel ircserv::_channels;

void ircserv::initialisation( char* pass, char* port ) {
	if ( strlen( port ) > 5 ) {
		_failed = true;
		logger( "ERROR", "port value too high !" );
		return;
	}
	_port = atoi( port );
	if ( _port > MAX_PORT || _port <= 0 || !check_num(port)) {
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
	std::cout << _clients.size() << std::endl;
	if (_clients.size() > 1020)
		return ;
	int fd = accept( _tcp6_socket, (sockaddr*) &addr, &len );
	if ( fd >= 0 ) {
		ircserv::_clients.push_back(Client( fd, addr ));
		Client&     ptr       = _clients.back();
		epoll_event event     = { EPOLLIN, { .ptr = &ptr } };
		epoll_ctl( _pollfd, EPOLL_CTL_ADD, fd, &event );
	} else
		logger( "ERROR", "accept error" );
}

void ircserv::process_events( epoll_event& ev ) {
	char           buf[512];
	Client*        c;
	ssize_t        len;
	MessageBuilder mb;
	if ( ev.events & EPOLLIN ) {
		if ( ev.data.fd == _tcp6_socket ) {
			accept_client( ev );
			// std::cout << ircserv::_clients.front().getFd() << std::endl;
		} else {
			c   = reinterpret_cast<Client*>( ev.data.ptr );
			len = read( c->getFd(), buf, 512 );
			if ( len < 0 ) {
				logger( "WARNING", strerror( errno ) );
				logger( "INFO", mb << "deleted: " << c->getFd() );
				return;
			}
			if ( len == 0 ) {
				logger( "INFO", mb << "deleted: " << c->getFd() );
				c->buf.clear();
				c->setDestroy();
				ircserv::removeClient( *c );
				return;
			}
			c->buf.append( buf, len );
			for ( ;; ) {
				size_t pos = c->buf.find( "\n" );
				if ( pos == std::string::npos )
					break;
				std::list<std::string>* args = parse( c->buf.substr( 0, pos ) );
				bool                    a;
				a = false;
				if ( args->front() == "QUIT" )
					a = true;
				handler( args, *c );
				delete args;
				if ( !a )
					c->buf.erase( 0, pos + 1 );
				else
					break;
				//			logger( "DEBUG", "buf after mdr : %s",
				// c->buf.c_str()
				//);
			}
		}
	} else if ( ev.events & EPOLLOUT ) {
		c   = reinterpret_cast<Client*>( ev.data.ptr );
		len = c->out.copy( buf, 512 );
		len = write( c->getFd(), buf, len );
		if ( len < 1 ) {
			logger( "WARNING", "useless event detected" );
		}
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
		logger( "ERROR", strerror( errno ) );
		return;
	}
	listen( _tcp6_socket, 256 );
	_clients.reserve(1024);
	// sockaddr_in6 peer_addr = {};
	// socklen_t len = sizeof(peer_addr);
	_pollfd           = epoll_create( 1 );
	epoll_event event = { EPOLLIN, { .fd = _tcp6_socket } };
	epoll_ctl( _pollfd, EPOLL_CTL_ADD, _tcp6_socket, &event );
	logger( "INFO", "server started successfuly" );
	// int b = 0;
	signal( SIGINT, interupt_handler );
	while ( !is_signaled ) {
		epoll_event events[64];
		int         nev = epoll_wait( _pollfd, events, 64, -1 );
		// logger( "DEBUG", "nev: %d", nev );
		for ( int i = 0; i < nev; i++ ) {
			process_events( events[i] );
		}
		// b++;
	}
}

void ircserv::stop( void ) {
	MessageBuilder mb;

	logger( "INFO", "safely ending ircserv !" );
	close( _tcp6_socket );
	close( _pollfd );
	logger("DEBUG", mb << std::distance(_clients.begin(), _clients.end()));
	forEach( _clients, close_client );
	_clients.clear();
	_channels.clear();
	exit(130);
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

t_client_array& ircserv::getClients( void ) {
	return _clients;
}

void ircserv::addChannel( std::string& name, Client& client ) {
	if ( _channels.find( name ) != _channels.end() )
		return;
	_channels.insert( std::make_pair( name, Channel( client, name ) ) );
}

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
	t_client_array::iterator it = _clients.begin();
	for ( ; it < _clients.end(); it++ ) {
		if ( it->getFd() == c.getFd() ) {
			epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_DEL, c.getFd(), NULL );
			close( c.getFd() );
			_clients.erase(it);
			break;
		}
	}
}
