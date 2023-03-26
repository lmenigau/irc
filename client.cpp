#include "client.hpp"
#include "ircserv.hpp"
void client::reply( std::string const str ) {
	if ( !ispolled ) {
		epoll_event event = { EPOLLOUT | EPOLLIN, { .ptr = this } };
		epoll_ctl( ircserv::getPollfd(), EPOLL_CTL_MOD, fd, &event );
	}
	out += str;
}
