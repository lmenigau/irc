#include "ostream.hpp"
#include <sys/epoll.h>
#include <ostream>

std::ostream& operator<<( std::ostream& os, epoll_event& ev ) {
	if ( ev.events & EPOLLHUP )
		os << "HUP";
	if ( ev.events & EPOLLIN )
		os << "IN";
	if ( ev.events & EPOLLOUT )
		os << "OUT";
	if ( ev.events & EPOLLRDHUP )
		os << "RDHUP";
	if ( ev.events & EPOLLPRI )
		os << "PRI";
	if ( ev.events & EPOLLERR )
		os << "ERR";
	os << ", " << ev.data.ptr << "\n";
	return os;
}
