#include "channel.hpp"
#include <iostream>
#include <string>
#include "client.hpp"
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "ostream.hpp"
#include "typedef.hpp"
#include "utils.hpp"

Channel::~Channel() {}
Channel::Channel( void ) : _limit( 0 ) {}

Channel::Channel( std::string name ) : _name( name ), _limit( 0 ) {}

Channel::Channel( Client& creator, const std::string& name )
    : _name( name ), _limit( 0 ) {
	_ops.push_back( &creator );
	_clients.push_back( &creator );
	_invite_only = false;
}

void Channel::addClient( Client& client ) {
	_clients.push_back( &client );
}

Channel::Channel( const Channel& a )
    : _clients( a._clients ),
      _modes( a._modes ),
      _name( a._name ),
      _topic( a._topic ),
      _password( a._password ),
      _ops( a._ops ),
      _banned( a._banned ),
      _key( a._key ),
      _invite_only( a._invite_only ),
      _limit( a._limit ) {}

void Channel::removeClient( Client& rclient ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	MessageBuilder                mb;

	while ( it != _clients.end() ) {
		if ( ( *it )->getNick() == rclient.getNick() ) {
			_clients.erase( it );
			return;
		}
		it++;
	}
	logger( "ERROR", mb << "user " << rclient.getUser()
	                    << " not found in channel " << _name << " !" );
}

void Channel::removeClient( Client& rclient, std::string msg ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	MessageBuilder                mb;

	while ( it != _clients.end() ) {
		if ( ( *it )->getNick() == rclient.getNick() ) {
			_clients.erase( it );
			sendAll( mb << ':' << rclient.getNick() << '!' << rclient.getUser()
			            << '@' << rclient.getHostname() << " PART " << _name
			            << " :" << msg <<"\r\n", rclient );
			return;
		}
		it++;
	}
	logger( "ERROR", mb << "user " << rclient.getUser()
	                    << " not found in channel " << _name << " !" );
}

void Channel::changeModes( int n_mode ) {
	(void) n_mode;
	(void) _modes;
	return;
}

void Channel::inviteUser( Client& c ) {
	_invited.push_back( &c );
}

bool Channel::isOps( Client& c ) {
	for ( t_vector_client_ptr::iterator it = _ops.begin(); it != _ops.end();
	      it++ ) {
		if ( c.getFd() == ( *it )->getFd() )
			return ( true );
	}
	return ( false );
}

t_vector_client_ptr& Channel::getClients( void ) {
	return _clients;
}

void Channel::setModes( std::string modes ) {
	_modes = modes;
}

std::string Channel::getModes( void ) {
	return _modes;
}

void Channel::setTopic( const std::string& topic ) {
	_topic = topic;
}

bool Channel::hasTopic( void ) {
	if ( !_topic.empty() )
		return ( true );
	return ( false );
}

std::string Channel::getTopic( void ) {
	return _topic;
}

std::string Channel::addModes( std::string modes ) {
	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( _modes.find( modes[i] ) != std::string::npos )
			continue;
		_modes.insert( modes.end(), modes[i] );
	}
	return ( _modes );
}

std::string Channel::removeModes( std::string modes ) {
	size_t it;

	for ( size_t i = 0; i < modes.size(); i++ ) {
		if ( ( it = _modes.find( modes[i] ) ) == std::string::npos )
			continue;
		_modes.erase( it );
	}
	return ( _modes );
}

void Channel::sendAll( MessageBuilder& mb ) {
	sendAll( mb.getBuff() );
	mb.clear();
}

void Channel::sendAll( std::string msg ) {
	// std::cout << "clients : " << _clients <<
	t_vector_client_ptr::iterator it = _clients.begin();
	for ( ; it != _clients.end(); it++ ) {
		( *it )->reply( msg );
	}
}

t_vector_client_ptr& Channel::getOps( void ) {
	return ( _ops );
}

void Channel::sendAll( MessageBuilder& mb, Client& c ) {
	sendAll( mb.getBuff(), c );
	mb.clear();
}

void Channel::sendAll( std::string msg, Client& c ) {
	t_vector_client_ptr::iterator it = _clients.begin();
	// std::cout << "clients : " << _clients << "\n";
	for ( ; it != _clients.end(); it++ ) {
		if ( ( *it )->getFd() != c.getFd() ) {
			( *it )->reply( msg );
		}
	}
}

bool Channel::findClients( const std::string& nick ) {
	t_vector_client_ptr::iterator it = this->_clients.begin();
	// std::cout << "clients : " << _clients << "\n";
	for ( ; it != this->_clients.end(); it++ ) {
		if ( ( *it )->getNick() == nick )
			return ( true );
	}
	return ( false );
}

void Channel::m_key( Client& c, std::string args, t_ope operation ) {
	(void) c;
	if ( operation == ADD )
		_key = args;
	else
		_key = "";
}

void Channel::m_invite( Client& c, std::string args, t_ope operation ) {
	if ( operation == ADD )
		_invite_only = true;
	else
		_invite_only = false;
	(void) c;
	(void) args;
}

void Channel::m_operator( Client& c, std::string args, t_ope operation ) {
	std::string    target;
	MessageBuilder mb;
	Client*        client;

	for ( size_t i = 0; i != args.size(); i++ ) {
		target = getTarget( i, args );
		if ( target.empty() )
			continue;
		client = find_client( target );
		if ( !client ) {
			c.reply( mb << ":" << ircserv::getServername() << " 441 "
			            << c.getNick() << " " << target
			            << " :They aren't on that channel\r\n" );
			continue;
		}
		t_vector_client_ptr::iterator it =
		    std::find( _ops.begin(), _ops.end(), client );
		if ( operation == ADD ) {
			client->reply( mb << ":" << c.getNick() << " MODE " << _name
			                  << " +o " << target << "\r\n" );
			if ( it == _ops.end() )
				_ops.push_back( client );
			c.reply( mb << ":" << ircserv::getServername() << " 324 "
			            << c.getNick() << " " << _name << " +o " << target
			            << "\r\n" );
		} else {
			if ( it == _ops.end() ) {
				c.reply( mb << ":" << ircserv::getServername() << " 441 "
				            << target << " " << _name
				            << " :They aren't not op on that channel\r\n" );
				continue;
			}
			_ops.erase( it );
			client->reply( mb << ":" << c.getNick() << " MODE " << _name
			                  << " -o " << target << "\r\n" );
			c.reply( mb << ":" << ircserv::getServername() << " 324 "
			            << c.getNick() << " " << _name << " -o " << target
			            << "\r\n" );
		}
	}
}

void Channel::m_limit( Client& c, std::string args, t_ope operation ) {
	MessageBuilder mb;

	if ( operation == ADD && isValidPositiveNumber( args ) )
		_limit = std::atoi( args.c_str() );
	else
		_limit = 0;
	c.reply( mb << ircserv::getServername() << " 324 " << c.getNick() << " "
	            << _name << " +l " << args << "\r\n" );
}

void Channel::reply_ban_list( Client& c ) {
	MessageBuilder mb;

	mb << ":ircserv.locahost 367 " << c.getNick() << " " << _name;
	for ( t_vector_client_ptr::iterator it = _banned.begin();
	      it != _banned.end(); it++ )
		mb << ( *it )->getNick() << "!" << ( *it )->getUser() << "@"
		   << ( *it )->getHostname() << " ";
	//	mb << ":Banned users\r\n";
	c.reply( mb );
	return ( c.reply( ":irvserv.localhost 368 :End of channel ban list\r\n" ) );
}

void Channel::m_ban( Client& c, std::string args, t_ope operation ) {
	if ( operation == NONE )
		return ( reply_ban_list( c ) );
	std::string    target;
	Client*        client;
	MessageBuilder mb;

	for ( size_t i = 0; i != args.size(); i++ ) {
		target = getTarget( i, args );
		if ( target.empty() )
			continue;
		client = find_client( target );
		if ( !findClients( target ) ) {
			c.reply( mb << ":" << ircserv::getServername() << " 441 " << target
			            << " " << _name << " :is not on that channel\r\n" );
			continue;
		}
		t_vector_client_ptr::iterator it =
		    std::find( _banned.begin(), _banned.end(), client );
		if ( operation == ADD ) {
			if ( it == _banned.end() )
				_banned.push_back( client );
			// client->reply( format( ":%s!%s@%s MODE %s %s :+b\r\n",
			//                        c.getNick().c_str(), c.getUser().c_str(),
			//                        c.getHostname().c_str(), _name.c_str(),
			//                        target.c_str() ) );
			// reply_ban_list( *client );

			//? not sure those rpl are necessary tho
			c.reply( mb << ':' << c.getNick() << '!' << c.getUser() << '@'
			            << c.getHostname() << " MODE " << _name << target
			            << " :+b\r\n" );

			reply_ban_list( c );
		} else {
			if ( it == _banned.end() ) {
				c.reply( mb << ":" << ircserv::getServername() << " 441 "
				            << target << " " << _name
				            << " :They aren't not banned on that channel\r\n" );
				continue;
			}
			_banned.erase( it );
			// client->reply( format( ":%s!%s@%s MODE %s: -b\r\n",
			// 						c.getNick().c_str(), c.getUser().c_str(),
			// 						c.getHostname().c_str(),
			// 						_name.c_str() ) );

			//?same here
			c.reply( mb << ':' << c.getNick() << '!' << c.getUser() << '@'
			            << c.getHostname() << " MODE " << _name << target
			            << " :-b\r\n" );
			reply_ban_list( c );
			// return ( reply_ban_list( c ) ); <- ???
		}
	}
}

void Channel::m_topic( Client& c, std::string args, t_ope operation ) {
	if ( operation == ADD )
		_topic_op = true;
	else
		_topic_op = false;
	(void) c;
	(void) args;
}

void Channel::handleModes( Client& c, std::string modes, std::string args ) {
	t_ope          operation;
	MessageBuilder mb;

	if ( !isOps( c ) ) {
		c.reply( mb << ":" << ircserv::getServername() << " 482 " << c.getNick()
		            << " :You're not cahnnel operator\r\n" );
		return;
	}

	if ( modes[0] == '+' )
		operation = ADD;
	else if ( modes[0] == '-' )
		operation = SUB;
	else
		operation = NONE;

	for ( std::string::iterator it = modes.begin(); it != modes.end(); it++ ) {
		if ( *it == '-' || *it == '+' )
			continue;
		switch ( *it ) {
			case 'k':
				m_key( c, args, operation );
				break;
			case 'i':
				m_invite( c, args, operation );
				break;
			case 'o':
				m_operator( c, args, operation );
				break;
			case 't':
				m_topic( c, args, operation );
				break;
			case 'b':
				m_ban( c, args, operation );
				break;
			case 'l':
				m_limit( c, args, operation );
				break;
			default:
				// DEBUG
				break;
		}
	}
}

bool Channel::getInviteMode( void ) {
	return _invite_only;
}

std::string& Channel::getKey( void ) {
	return _key;
}

bool Channel::isInvited( Client* c ) {
	if ( std::find( _invited.begin(), _invited.end(), c ) != _invited.end() )
		return ( true );
	return ( false );
}

bool Channel::isBanned( Client* c ) {
	if ( std::find( _banned.begin(), _banned.end(), c ) != _banned.end() )
		return ( true );
	return ( false );
}

bool Channel::isFull( void ) {
	if ( (int) _clients.size() > _limit && _limit > 0 )
		return ( true );
	return ( false );
}
