#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

void	kick( std::list<std::string>* args, Client &c)
{
	MessageBuilder mb;

	if (args->empty())
		return ;
	std::string   channel_name = args->front();
	args->pop_front();
	Channel *target = find_channel(channel_name);
	if (!target)
		return (c.reply(mb << ":" << ircserv::getServername() << " 403 " << channel_name << " :No such channel\r\n"));
	else if (args->size() < 2)
		return (c.reply(mb << ":" << ircserv::getServername() << " 461 KICK" << " :Not enough parameters\r\n"));
	else if (!target->isOps(c))
		return (c.reply(mb << ":" << ircserv::getServername() << " 482 " << channel_name << " :You're not channel operator\r\n"));
	else if (!target->findClients(c.getNick()))
		return (c.reply(mb << ":" << ircserv::getServername() << " 442 " << channel_name <<
		" :You're not on that channel\r\n"));
	size_t	i = 0;
	std::string target_user;
	std::string comment = "";

	if (args->size() > 1)
		comment = args->back();
	while (i != args->front().size())
	{
		target_user = getTarget( i, args->front());
		if (target_user.empty())
			continue ;
		if (!target->findClients(target_user))
			c.reply(mb << ":" << ircserv::getServername() << " 441 " << target_user << " " << channel_name <<
			" :They aren't on that channel\r\n");
		else
		{
			target->sendAll(mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " KICK " << channel_name << " " << target_user << " " << comment << "\r\n");
			target->removeClient(*(find_client(target_user)));
		}
	}
}
