#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#define PASSWORD "pass"
/*
//Same as #chan +o #nick
static void oper_on_channel(std::list <std::string> *args, Client &c)
{
    Channel *channel = find_channel( args->front() );
    if (!channel)
        return (c.reply( format (":ircserv.locahost 403 %s %s :No such
channel\r\n", c.getNick().c_str(), args->front().c_str()))); if
(channel->isOps(c)) return (c.reply(format (":ircserv.localhost 482 %s %s :You
are operator on this channel\r\n", c.getNick().c_str(),
args->front().c_str())));
}*/

static void oper_on_serv( std::list<std::string>* args, Client& c ) {
	if ( c.isServerOp() )
		return ( c.reply( format(
		    ":ircserv.localhost 381 %s :You are already an IRC operator\r\n",
		    c.getNick().c_str() ) ) );
	if ( args->back() == PASSWORD ) {
		c.setOp();
		return ( c.reply( format(
		    ":ircserv.localhost 381 %s :You now are an IRC operator \r\n",
		    c.getNick().c_str() ) ) );
	} else
		return ( c.reply( ":ircserv.locahost 464 :Password incorrect \r\n" ) );
}

void oper( std::list<std::string>* args, Client& c ) {
	if ( args->size() != 2 )
		return c.reply(
		    ":ircserv.locahost 461 OPER :Not enough parameters\r\n" );
	// if (args->front().c_str()[0] == '#')
	//	oper_on_channel(args, c);
	// else
	oper_on_serv( args, c );
}
