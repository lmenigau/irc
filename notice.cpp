#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

void notice(std::list <std::string> *args, Client &c) {
	if ( args->empty() )
		return;
	std::string target = args->front();
	//std::cout << target << std::endl;
	while (args->size() != 1)
	{
	//	std::cout << "args :" << args->front() << std::endl;
		if (isChannel(args->front()))
		{
			Channel* channel = find_channel( args->front() );
			if (channel)
				channel->notice(args, c);
		}
		else
			notice_client(args, c);
		args->pop_front();
	}

}
