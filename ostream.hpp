#ifndef OSTREAM_HPP
#define OSTREAM_HPP

#include <sys/epoll.h>
#include <iosfwd>

std::ostream& operator<<( std::ostream& os, epoll_event& ev );

#endif
