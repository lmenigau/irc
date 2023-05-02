#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

static bool if_nick_set( const std::string& name ) {
	for ( t_map_client::iterator it = ircserv::getClients().begin();
	      it != ircserv::getClients().end(); it++ ) {
		if ( it->second.getNick() == name )
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

void whois( std::list<std::string>* args, Client &c) {
	if ( args->empty() )
		return;
	return;
	if ( if_nick_set( args->front() ) ) {
		c.reply( format(
		    ":ircserv.localhost 307 %s: has identified for this nick\r\n",
		    c.getNick().c_str() ) );
		c.reply( format( ":irvserv.localhost 311 %s %s %s* : %s\r\n",
		                 c.getNick().c_str(), c.getUser().c_str(),
		                 c.getHostname().c_str(), c.getRealUser().c_str() ) );
		// May need a function get serverHost
		c.reply(
		    format( ":ircserv.localhost 312 %s ft_irc: Our IRC Server, the "
		            "best ofc\r\n",
		            c.getNick().c_str() ) );
		// If operator :
		c.reply( format( ":ircserv.localhost 313 %s: is an IRC operator\r\n",
		                 c.getNick().c_str() ) );
		// Varible of time to konw from how long the client is connected and
		// since when he finished
		c.reply( ( format( ":ircserv.localhost 317: 1 1" ) ) );
		c.reply( ( format( ":ircserv.localhost 330 %s %s: is logged in as\r\n",
		                   c.getNick().c_str(), c.getUser().c_str() ) ) );
		c.reply( (
		    format(
		        ":ircserv.localhost 671 %s: is using a secure connection\r\n" ),
		    c.getNick().c_str() ) );
		c.reply(
		    ( format( ":ircserv.localhost 318 %s: End of /WHOIS list.\r\n" ),
		      c.getNick().c_str() ) );
	}
}
