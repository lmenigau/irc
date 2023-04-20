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
	for ( std::vector<Client *>::iterator it = ircserv::_clients.begin();
	      									it != ircserv::_clients.end(); it++ ) {
		if ( ( *it )->getNick() == args->front() ) {
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
}

void	privmsg_channel(std::list <std::string> * args, Client *c, const std::string &target)
{
	std::map<std::string, Channel>           channels = ircserv::getChannels();
	std::map<std::string, Channel>::iterator it = channels.find(target);

	std::cout << "WTFFF"	 << std::endl;
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
		std::cout << "??" << std::endl;
		for (std::vector<Client *>::iterator cli_list = it->second.getClients().begin();
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
	if (target[0] == '#')
		privmsg_channel(args, c, target);
	else
		privmsg_client(args, c, target);
	// void *buf = (const_cast<char *>(args->back().c_str()));
	
	logger( "INFO", "%s wants to send a message", c->getNick().c_str() );
}