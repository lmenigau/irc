#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "client.hpp"

class Channel {
   private:
	std::map<std::string, Client> _clients;
	std::string                   _name;
	std::string                   _topic;
	std::string                   _password;
	std::string                   _modes;
	Client                        _admin;
	Channel();

   public:
	Channel( std::string );
	Channel( Client& creator, const std::string& name );
	~Channel();
	void addClient( Client );
	void removeClient( Client );
	void changeModes( int );

	std::string getModes( void );

	void                           setModes( std::string );
	std::map<std::string, Client>& getClients( void );
};

#endif
