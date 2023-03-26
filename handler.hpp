#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <list>
#include <string>
#include "client.hpp"
#include "irc.hpp"

void handler( std::list<std::string>* args, client& c );

#endif
