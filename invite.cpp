#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void  invite( std::list<std::string> *args, Client &c)
{
	if (args->size() == 1 || args->size() == 0)
	{
		c.reply( ":ircserv.localhost 461 INVITE :Not enough parameters\r\n" );
		return ;
	}
	Channel *channel_target = find_channel(args->back());
	if (!channel_target)
		return (c.reply( format(":ircserv.localhost 403 %s :No such channel\r\n", args->back().c_str())));
	if (!channel_target->findClients(c.getNick()))
		return (c.reply( format(":ircserv.localhost 442 %s :You're not on that channel\r\n", args->back().c_str())));
	//Priviliges part : depending on modes, not done for the moment SETUP LIKE NEED TO BE OP CHANNEL ANY WAY
	while (args->size() != 1)
	{
		Client *client_target = find_client(args->front());
		if (!client_target)
			c.reply( format (":ircserv.localhost 401 %s :No such nick\r\n", args->front().c_str()));
		else if (channel_target->findClients(args->front()))
			c.reply( format (":ircserv.localhost 443 %s %s :is already on channel\r\n", args->front().c_str(), args->back().c_str()));
		else
		{
			c.reply( format (":ircserv.localhost 341 %s %s %s\r\n", c.getNick().c_str(), args->front().c_str(), args->back().c_str()));
			client_target->reply( format(":%s!~%s@%s INVITE %s %s\r\n", c.getNick().c_str(), c.getUser().c_str(), c.getHostname().c_str(), args->front().c_str(), args->back().c_str()));
			channel_target->inviteUser(client_target);
		}
		args->pop_front();
	}
	for (t_map_channel::iterator it=ircserv::getChannels().begin(); it != ircserv::getChannels().end(); it++)
	{
			std::cout << "List :" << &(it->second) << std::endl;
	}
}
