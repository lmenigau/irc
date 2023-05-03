#include <list>
#include "ircserv.hpp"
#include "utils.hpp"

static void reply_to_join(const std::string &channel_name, Client &c,
                             t_map_channel::iterator it)
{
	std::string reply;
	
	reply = format(":ircserv.localhost 353 %s = %s :", c.getNick().c_str(),
	                                                    channel_name.c_str());
	for (t_vector_client_ptr::iterator it_member = it->second.getClients().begin();
	                                                        it_member != it->second.getClients().end();
	                                                        it_member++) 
	{
					if (it->second.isOps(**it_member))
		        reply.append("@" + ( *it_member)->getNick() + " ");
					else
		        reply.append(( *it_member)->getNick() + " ");
	}
	reply.append("\r\n");
	c.reply(reply);
	c.reply(reply);
}

void join( std::list<std::string>* args, Client& c ) {
	t_map_channel           channels = ircserv::getChannels();
	t_map_channel::iterator it;

	if (args->front().c_str()[0] != '#')
	{ c.reply( format( ":ircserv.localhost 403 %s : No such channel\n\r", args->front().c_str()));
		return ;
	}
	logger( "INFO", "%s joined channel %s", c.getNick().c_str(),
	        args->front().c_str() );
	it = channels.find( args->front() );
	if ( it == channels.end() ) {
		ircserv::addChannel( args->front(), c );
		it = ircserv::getChannels().find( args->front() );
	} else
		ircserv::getChannels()
		    .find( args->front() )
		    ->second.getClients()
		    .push_back(&c);
	it->second.sendAll( format( ":%s!%s JOIN %s\r\n", c.getNick().c_str(),
	                            c.getHostname().c_str(),
	                            args->front().c_str() ) );
	reply_to_join(args->front(), c, it);
	c.reply( format( ":ircserv.localhost 366 %s %s :End of NAMES list\r\n",
	                 c.getUser().c_str(), args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 332 %s :no topic set\r\n",
	                 args->front().c_str() ) );
	c.reply( format( ":ircserv.localhost 353 : :\r\n" ) );
}
