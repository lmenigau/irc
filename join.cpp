#include <list>
#include "ircserv.hpp"
#include "messageBuilder.hpp"
#include "utils.hpp"

static void reply_to_join( const std::string&      channel_name,
                           Client&                 c,
                           t_map_channel::iterator it ) {
	MessageBuilder reply;

	reply << ":" << ircserv::getServername() << " 353 " << c.getNick() << " = "
	      << channel_name << " :";
	for ( t_vector_client_ptr::iterator it_member =
	          it->second.getClients().begin();
	      it_member != it->second.getClients().end(); it_member++ ) {
		if ( it->second.isOps( **it_member ) )
			reply += ( "@" + ( *it_member )->getNick() + " " );
		else
			reply += ( ( *it_member )->getNick() + " " );
	}
	reply += ( "\r\n" );
	c.reply( reply );
}

static void reply_topic( t_map_channel::iterator it, Client& c ) {
	MessageBuilder mb;
	if ( it->second.hasTopic() )
		c.reply( mb << ":" << ircserv::getServername() << " 332 " << c.getNick()
		            << " " << it->first << " :" << it->second.getTopic()
		            << "\r\n" );
	else
		c.reply( mb << ":" << ircserv::getServername() << " 331 " << c.getNick()
		            << " " << it->first << " :No topic is set\r\n" );
}

void join( std::list<std::string>* args, Client& c ) {
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it;
	MessageBuilder          mb;
	if ( !isChannel( args->front() ) ) {
		c.reply( mb << ":" << ircserv::getServername() << " 403 "
		            << args->front() << " :No such channel\r\n" );
		return;
	}
	logger( "INFO", mb << c.getNick() << " joined channel " << args->front() );
	it = channels.find( args->front() );
	if ( it == channels.end() ) {
		ircserv::addChannel( args->front(), c );
		it = ircserv::getChannels().find( args->front() );
	} else {
		if ( !it->second.getKey().empty() &&
		     args->back() != it->second.getKey() )
			return ( c.reply( mb
			                  << ":" << ircserv::getServername() << " 475 "
			                  << c.getNick() << " " << args->front()
			                  << " :Cannot join channel (+k)\r\n" ) );  // 475
		else if ( it->second.isBanned( &c ) )
			return ( c.reply( mb
			                  << ":" << ircserv::getServername() << " 474 "
			                  << c.getNick() << " " << args->front()
			                  << " :Cannot join channel (+b)\r\n" ) );  // 474
		else if ( it->second.getInviteMode() &&
		          !ircserv::getChannels().at( args->front() ).isInvited( &c ) )
			return ( c.reply( mb
			                  << ":" << ircserv::getServername() << " 473 "
			                  << c.getNick() << " " << args->front()
			                  << " :Cannot join channel (+i)\r\n" ) );  // 473
		else if ( it->second.isFull() )
			return ( c.reply( mb
			                  << ":" << ircserv::getServername() << " 471 "
			                  << c.getNick() << " " << args->front()
			                  << " :Cannot join channel (+l)\r\n" ) );  // 471

		ircserv::getChannels()[args->front()].addClient( c );
	}
	reply_topic( it, c );
	it->second.sendAll( mb << ":" << c.getNick() << "!" << c.getUser() << "@"
	                       << c.getHostname() << " JOIN " << args->front()
	                       << "\r\n" );  // 332
	reply_to_join( args->front(), c, it );
	c.reply( mb << ":" << ircserv::getServername() << " 366 " << c.getNick()
	            << " " << args->front() << " :End of NAMES list\r\n" );  // 366
}
