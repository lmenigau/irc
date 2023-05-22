#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"
#include "messageBuilder.hpp"

static bool if_nick_set( const std::string& name ) {
	for ( t_client_array::iterator it = ircserv::getClients().begin();
	      it != ircserv::getClients().end(); it++ ) {
		if ( it->getNick() == name )
			return ( true );
	}
	return ( false );
}

// Function check/get certificate fingerprint connexion ? (i guess not useful)
// Function checking if operator or not
// Variable time in client class, time since creation/ time since last update
// Who is actually, displaying host and ip
// Who is channel, status of the nick in the channel / all channel and status
// display Who is host, displaying the host name / ip 378 Who is mode,
// displaying the modes 379 RPL_AWAY  301, is away

void whois( std::list<std::string>* args, Client& c ) {
	MessageBuilder mb;
	if ( args->empty() )
		return;
	return;
	if ( if_nick_set( args->front() ) ) {
		c.reply( mb << ':' << ircserv::getServername() << " 307 " << c.getNick() << " : has identified for this nick\r\n" );
		mb.clear();
		c.reply( mb << ':' << ircserv::getServername() << " 311 " << c.getNick() << " " << c.getUser() << " " << c.getHostname() << " * :" << c.getRealUser() << "\r\n" );
		mb.clear();
		// May need a function get serverHost
		c.reply( mb << ':' << ircserv::getServername() << " 312 " << c.getNick() << " " << ircserv::getServername() << " :Our IRC Server, the best ofc\r\n" );
		mb.clear();
		// If operator :
		c.reply( mb << ':' << ircserv::getServername() << " 313 " << c.getNick() << " :is an IRC operator\r\n" );
		mb.clear();
		// Varible of time to konw from how long the client is connected and
		// since when he finished
		c.reply( mb << ':' << ircserv::getServername() << " 317 " << c.getNick() << " " << "1" << " :seconds idle, signon time\r\n" );
		mb.clear();
		c.reply( mb << ':' << ircserv::getServername() << " 330 " << c.getNick() << " " << c.getUser() << " :is logged in as\r\n" );
		mb.clear();
		c.reply( mb << ':' << ircserv::getServername() << " 671 " << c.getNick() << " :is using a secure connection\r\n" );
		mb.clear();
		c.reply( mb << ':' << ircserv::getServername() << " 318 " << c.getNick() << " :End of /WHOIS list.\r\n" );
	}
}
