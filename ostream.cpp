#include "ostream.hpp"
#include <sys/epoll.h>
#include <ostream>
#include "client.hpp"
#include <map>

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

std::ostream& operator<<( std::ostream& os, std::map<std::string, Client *> map) {
	std::map<std::string, Client *>::iterator it = map.begin();
	for(; it != map.end(); it++) {
		os << it->second->getNick() << " ";
	}
	return os;
}
