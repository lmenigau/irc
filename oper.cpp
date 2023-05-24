#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"
#include "ircserv.hpp"
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
	MessageBuilder mb;
	if ( c.isServerOp() )
		return ( c.reply( mb << ":ircserv.localhost 381 " << c.getNick()
		                  << " :You are already an IRC operator\r\n" ) );
	if ( args->back() == PASSWORD ) {
		c.setOp();
		c.reply( mb << ':'
		                  << "ircserv.localhost 381 " << c.getNick()
		                  << " :You now are an IRC operator\r\n" );
		c.addModes("o");
		c.reply( mb << ':' << ircserv::getServername() << " 324 " << c.getNick()
		            << " " << c.getNick() << " " << c.getModes() << "\r\n" );
	} else
		return ( c.reply( mb << ':' << ircserv::getServername()
		                  << " 464 :Password incorrect\r\n" ) );
}

void oper( std::list<std::string>* args, Client& c ) {
	MessageBuilder mb;
	if ( args->size() != 2 )
		return c.reply( mb << ':' << ircserv::getServername()
		                 << " 461 OPER :Not enough parameters\r\n" );
	// if (args->front().c_str()[0] == '#')
	//	oper_on_channel(args, c);
	// else
	oper_on_serv( args, c );
}
