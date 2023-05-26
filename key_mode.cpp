#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

void Channel::m_key( Client& c, std::string args, t_ope operation ) {
	(void) c;
	MessageBuilder mb;

	if ( operation == ADD )
	{
		_key = args;
		if (args.empty())
			return (reply_334(c));
		addModes("k");
		this->sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			<< " +k " << args << "\r\n" );
	}
	else
	{
		removeModes("k");
		this->sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			<< " -k " << "\r\n" );
		_key = "";
	}
}

std::string& Channel::getKey( void ) {
	return _key;
}
