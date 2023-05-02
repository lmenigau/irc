#include "utils.hpp"
#include "ircserv.hpp"
#include <list>

void join( std::list<std::string>* args, Client& c ) {
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it;
	logger( "INFO", "%s joined channel %s", c.getNick().c_str(),
	        args->front().c_str() );
	it = channels.find( args->front() );
	if ( it == channels.end() ) {
		ircserv::addChannel( args->front(), c );
		it = ircserv::getChannels().find( args->front() );
	} else
		it->second.addClient( c );
	it->second.sendAll( format( ":%s!%s JOIN %s\r\n", c.getNick().c_str(),
	                             c.getHostname().c_str(),
	                             args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 353 %s = %s :@%s\r\n",
	                 c.getUser().c_str(), args->front().c_str(),
	                 c.getNick().c_str() ) );
	c.reply( format( ":ircserv.localhost 366 %s %s :End of NAMES list\r\n",
	                 c.getUser().c_str(), args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 332 :%s :no topic\r\n",
	                 args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 353 : :\r\n" ) );
}
