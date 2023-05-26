#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

void Channel::m_topic( Client& c, std::string args, t_ope operation ) {
	MessageBuilder mb;

	if ( operation == ADD )
	{
		if (_topic_op)
			return (reply_334(c));
		_topic_op = true;
		sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			<< " +t " << "\r\n" );
	}
	else
	{
		if (!_topic_op)
			return (reply_334(c));
		_topic_op = false;
		sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			<< " -t " << "\r\n" );
	}
	(void) args;
}

std::string Channel::getTopic( void ) {
	return _topic;
}

void Channel::setTopic( const std::string& topic ) {
	_topic = topic;
}

bool Channel::hasTopic( void ) {
	if ( !_topic.empty() )
		return ( true );
	return ( false );
}

bool         Channel::topicRight(void)
{
	return (_topic_op);
}
