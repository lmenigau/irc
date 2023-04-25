#ifndef OSTREAM_HPP
#define OSTREAM_HPP

#include <sys/epoll.h>
#include <iosfwd>
#include <map>
#include "client.hpp"
std::ostream& operator<<( std::ostream& os, epoll_event& ev );
std::ostream& operator<<( std::ostream& os, std::map<std::string, Client *> map);

#endif
