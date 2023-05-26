#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

void Channel::m_limit( Client& c, std::string args, t_ope operation ) {
	MessageBuilder mb;

	_limit = 0;
	if ( operation == ADD && isValidPositiveNumber( args ) && !args.empty() )
	{
		_limit = std::atoi( args.c_str() );
		sendAll( mb << ":"  << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name << " +l " << _limit << "\r\n");
		addModes("l");
	}
	else
	{
		_limit = 0;
		reply_334(c);
	}
	if (operation == SUB)
	{
		sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name << " -l \r\n");
		removeModes("l");
	}
}

bool Channel::isFull( void ) {
	if ( (int) _clients.size() >= _limit && _limit > 0 )
		return ( true );
	return ( false );
}

