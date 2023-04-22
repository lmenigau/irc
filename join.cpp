#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

static void reply_to_join(const std::string &channel_name, Client *c,
                             t_map_channel::iterator it)
{
    std::string reply;

    reply = format(":ircserv.localhost 353 %s = %s :@", c->getNick().c_str(),
                                                        channel_name.c_str());
    for (t_vector_client_ref::iterator it_member = it->second.getClients().begin();
                                                            it_member != it->second.getClients().end();
                                                            it_member++) {
            reply.append(( *it_member)->getNick() + " ");
    }
    reply.append("\r\n");
    c->reply(reply);
}

static void announce_new_client( std::string name, Client *c, std::map<std::string, Channel>::iterator it)
{
	t_vector_client_ref list = it->second.getClients();
	for (t_vector_client_ref::iterator it = list.begin(); it != list.end(); it++)
	{
		if (*it != c)
			( *it )->reply( format( ":%s!%s@%s JOIN %s\r\n", c->getNick().c_str(), c->getUser().c_str(), c->getHostname().c_str(), name.c_str()));
	}
}

void join( std::list<std::string>* args, Client *c)
{
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it;
	/// args->front().erase( args->front().length() - 1, 1 );
	logger( "INFO", "%s joined channel %s", c->getNick().c_str(),
	        args->front().c_str() );
	it = channels.find( args->front() );
	if ( it == channels.end() ) {
		ircserv::addChannel( args->front(), *c);
		it = ircserv::getChannels().find( args->front() );
	}
	else 
		  it->second.addClient( c );
	c->reply( format( ":%s!foo.example.bar JOIN %s\r\n", c->getNick().c_str(),
	                 args->front().c_str() ) );
    reply_to_join(args->front(), c, it);
	c->reply( format( ":ircserv.localhost 366 %s %s :End of NAMES list\r\n",
	                 c->getUser().c_str(), args->front().c_str() ) );
	announce_new_client( args->front(), c, it);
	c->reply( format( ":ircserv.localhost 332 :%s :no topic\r\n",
	                 args->front().c_str() ) );
//	c->reply( format( ":ircserv.localhost 353 : :\r\n" ) );
}
