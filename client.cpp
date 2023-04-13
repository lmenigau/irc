#include "client.hpp"
#include <sys/epoll.h>
#include "ircserv.hpp"


void Client::reply( std::string const &str ) {
	if ( !ispolled ) {
		epoll_event event = { EPOLLOUT | EPOLLIN, { .ptr = this } };
		epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_MOD, fd, &event );
	}
	out += str;
}

Client::Client()
{

}

Client::~Client()
{
}

