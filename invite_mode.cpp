#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

bool Channel::getInviteMode( void ) {
	return _invite_only;
}

void Channel::inviteUser( Client& c ) {
	_invited.push_back( &c );
}


bool Channel::isInvited( Client* c ) {
	if ( std::find( _invited.begin(), _invited.end(), c ) != _invited.end() )
		return ( true );
	return ( false );
}

void Channel::m_invite( Client& c, std::string args, t_ope operation ) {
	MessageBuilder mb;
	if ( operation == ADD )
	{
		if (_invite_only)
			return (reply_334(c));
		addModes("i");
		this->sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			<< " +i " << "\r\n" );
		_invite_only = true;
	}
	else
	{
		if (!_invite_only)
			return (reply_334(c));
		removeModes("i");
		sendAll( mb << ":" << c.getNick() << "!~" << c.getUser() << "@" << c.getHostname() << " MODE " << _name
			<< " -i " << "\r\n" );
		_invite_only = false;
	}
	(void) args;
}
