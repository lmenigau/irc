#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

static void reply_to_join(const std::string &channel_name, Client *c,
                             std::map<std::string, Channel>::iterator it)
{
    std::string reply;

    reply = format(":ircserv.localhost 353 %s = %s :@", c->getNick().c_str(),
                                                        channel_name.c_str());
    for (std::vector<Client *>::iterator it_member = it->second.getClients().begin();
                                                            it_member != it->second.getClients().end();
                                                            it_member++) {
            reply.append(( *it_member)->getNick() + " ");
    }
    reply.append("\r\n");
    c->reply(reply);
}

void join( std::list<std::string>* args, Client *c)
{
	std::map<std::string, Channel>           channels = ircserv::getChannels();
	std::map<std::string, Channel>::iterator it;
	/// args->front().erase( args->front().length() - 1, 1 );
	logger( "INFO", "%s joined channel %s", c->getNick().c_str(),
	        args->front().c_str() );
	it = channels.find( args->front() );
	if ( it == channels.end() ) {
		ircserv::addChannel( args->front(), *c);
		it = ircserv::getChannels().find( args->front() );
	}
	else 
		{
            std::cout << it->first << std::endl;		    
		  it->second.addClient( c );
		}
	c->reply( format( ":%s!foo.example.bar JOIN %s\r\n", c->getNick().c_str(),
	                 args->front().c_str() ) );
    reply_to_join(args->front(), c, it);
	c->reply( format( ":ircserv.localhost 366 %s %s :End of NAMES list\r\n",
	                 c->getUser().c_str(), args->front().c_str() ) );
	c->reply( format( ":ircserv.localhost 332 :%s :no topic\r\n",
	                 args->front().c_str() ) );
//	c->reply( format( ":ircserv.localhost 353 : :\r\n" ) );
}
