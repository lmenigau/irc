#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"


//From my point of view only OPs or Creator should be able to change the topic of a channel.
void    topic(std::list <std::string> * args, Client *c)
{
    
}