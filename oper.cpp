#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"
#include "ircserv.hpp"
#define PASSWORD "pass"

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
		c.reply( mb << ':' << ircserv::getServername() << " 221 " << c.getNick()
		            << " " << c.getModes() << "\r\n" );
	} else
		return ( c.reply( mb << ':' << ircserv::getServername()
		                  << " 464 :Password incorrect\r\n" ) );
}

void oper( std::list<std::string>* args, Client& c ) {
	MessageBuilder mb;
	if ( args->size() != 2 )
		return c.reply( mb << ':' << ircserv::getServername()
		                 << " 461 OPER :Not enough parameters\r\n" );
	oper_on_serv( args, c );
}
