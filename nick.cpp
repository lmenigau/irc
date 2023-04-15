//Function to check if the nick is already in the db or not
//If it's the case return 433 "<client> <nick> :Nickname is already in use"

//We can define our accepted character list, if not corresping, 
//return 432 "<client> <nick> :Erroneus nickname"

#include <unistd.h>
#include <iostream>
#include <list>
#include "client.hpp"
#include "irc.hpp"
#include "ircserv.hpp"
#include "utils.hpp"

static bool  authorize_setting_name(const std::string &name)
{
  for (std::vector<Client *>::iterator it = ircserv::_clients.begin(); it != ircserv::_clients.end(); it++)
  {
    if ((*it)->getNick() == name)
      return (false);
  }
  return (true);
}

void nick( std::list<std::string> *args, Client &c)
{
  std::string  buff;

	if ( args->empty() || args->front() == "") {
		c.reply( format( ":ircserv.localhost 431 *  NICK: Not enough parameters\r\n" ));
	} 
  else if (!authorize_setting_name(args->front())) {
    c.reply( format( ":ircserv.localhost 433 * %s: Nickname is already in use\r\n",
      args->front().c_str()));
  }
  else if (c.hasGivenNick()){
    buff = c.getNick();
    c.setNick(args->front());
    c.reply(format ("%s!~%s@%s NICK %s\r\n", buff.c_str(),
      c.getUser().c_str(), c.getHostname().c_str(), c.getNick().c_str()));
		logger( "INFO", "User %s nickname change to %s.",
        c.getUser().c_str(), c.getNick().c_str());

  }
  else if (c.hasGivenUser()){
    c.setNick(args->front());
    c.setHasGivenNick(true);
    c.reply(
	  format( ":ircserv.localhost 002 %s :Your host is FT_IRC running "
	          "version 0.0.1dev\r\n",
	          c.getNick().c_str() ) );
	  c.reply( format(
	      ":ircserv.localhost 003 %s :This server was created idk like now ?\r\n",
	      c.getNick().c_str() ) );
	  c.reply( format( ":ircserv.localhost 004 %s :FT_IRC 0.0.1dev ia i\r\n",
	                   c.getNick().c_str() ) );
		logger( "INFO", "New user %s nickname %s set, Connexion etablished !",
        c.getUser().c_str(), c.getNick().c_str());
  }
}
