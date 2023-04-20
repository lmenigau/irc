#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void privmsg( std::list<std::string>* args, Client *c) {
	if ( args->empty() )
		return;
	// void *buf = (const_cast<char *>(args->back().c_str()));
	for ( std::vector<Client *>::iterator it = ircserv::_clients.begin();
	      it != ircserv::_clients.end(); it++ ) {
		if ( ( *it )->getNick() == args->front() ) {
			std::string target = args->front();
			args->front().append( "@" + ( ( *it )->getHostname() ) );
			std::cout << args->front() << " : " << args->back() << std::endl;
			// std::cout << send((*it)->getFd(), buf, args->back().length(), 0)
			// << std::endl;
			( *it )->reply(
			    format( ":%s!~%s PRIVMSG %s: %s\r\n", c->getNick().c_str(),
			            ( c->getUser() + "@" + c->getHostname() ).c_str(),
			            target.c_str(), args->back().c_str() ) );
		}
	}
	logger( "INFO", "%s wants to send a message", c->getNick().c_str() );
}
