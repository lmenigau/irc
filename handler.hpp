#ifndef HANDLER_HPP
#define HANDLER_HPP

#include <list>
#include <string>
#include "client.hpp"
#include "irc.hpp"

void handler( std::list<std::string>* args, Client& c );
bool authorize_setting_name( const std::string& name, Client& c );

void privmsg( std::list<std::string>* args, Client& c );
void nick( std::list<std::string>* args, Client& c );
void user( std::list<std::string>* args, Client& c );
void whois( std::list<std::string>* args, Client& c );
void quit( std::list<std::string>* args, Client& c );
void part( std::list<std::string>* args, Client& c );
void pass( std::list<std::string>* args, Client& c );
void join( std::list<std::string>* args, Client& c );
void mode( std::list<std::string>* args, Client& c );
void topic(std::list <std::string>* args, Client& c);
void notice(std::list <std::string> *args, Client &c);

void nick_notregistered( std::list<std::string>* args, Client& c );

#endif
