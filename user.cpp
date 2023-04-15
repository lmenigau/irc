#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void user( std::list<std::string>* args, Client& c ) {
	(void) args;
    if (args->empty())
    {
        c.reply(format(":ircserv.localhost * 461 USER: Not enough parameters\r\n"));
        return ;
    }
    else if (c.hasGivenUser())
    {
         c.reply(format(":ircserv.localhost * 462: You may not reregister\r\n"));
        return ;
    }
	logger( "INFO", "client %d has username %s", c.getFd(),
	        args->front().c_str() );
	c.setHasGivenUser( true );
	c.setUser( args->front() );
    //Checker la command 0 *: (real name)
    c.setRealUser(args->back());
	c.reply( format("~%s\r\n", c.getUser().c_str()));
}
