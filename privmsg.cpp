#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void	privmsg_client(std::list <std::string > * args, Client *c, const std::string &target)
{
	for ( t_map_int_client::iterator it = ircserv::_clients.begin();
	      									it != ircserv::_clients.end(); it++ ) {
		if ( it->second.getNick() == args->front() ) {
			args->front().append( "@" + ( it->second.getHostname() ) );
			std::cout << args->front() << " : " << args->back() << std::endl;
			// std::cout << send((*it)->getFd(), buf, args->back().length(), 0)
			// << std::endl;
			it->second.reply(
			    format( ":%s!~%s PRIVMSG %s: %s\r\n", c->getNick().c_str(),
			            ( c->getUser() + "@" + c->getHostname() ).c_str(),
			            target.c_str(), args->back().c_str() ) );
		}
	}
}

void	privmsg_channel(std::list <std::string> * args, Client *c, const std::string &target)
{
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it = channels.find(target);

	if (it == channels.end())
	{
		c->reply( format ("%s!~%s@%s 403 %s :No such channel\r\n", c->getNick().c_str(),
															c->getUser().c_str(),
															c->getHostname().c_str(),
															target.c_str()));
		return ;
	}
	else
	{
		for (t_vector_client_ref::iterator cli_list = it->second.getClients().begin();
															cli_list != it->second.getClients().end();
															cli_list++) {
			std::string rep = format( ":%s!~%s@%s PRIVMSG %s: %s\r\n", c->getNick().c_str(),
																	c->getUser().c_str(), c->getHostname().c_str(),
																	target.c_str(), args->back().c_str());
			std::cout << rep << std::endl;
			( *cli_list)->reply( format( ":%s!~%s@%s PRIVMSG %s : %s\r\n", c->getNick().c_str(),
																	c->getUser().c_str(), c->getHostname().c_str(),
																	target.c_str(), args->back().c_str()));
		}													
	}														
}

void privmsg( std::list<std::string>* args, Client *c) {
	if ( args->empty() )
		return;
	std::string target = args->front();
	std::cout << target << std::endl;
	if (isChannel(target))
		privmsg_channel(args, c, target);
	else
		privmsg_client(args, c, target);
	// void *buf = (const_cast<char *>(args->back().c_str()));
	
	logger( "INFO", "%s wants to send a message", c->getNick().c_str() );
}