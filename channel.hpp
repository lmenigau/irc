#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "client.hpp"

class channel {
   private:
	std::vector<client> _clients;
	int                 _modes;
	std::string         _name;
	channel();

   public:
	channel( std::string );
	~channel();
	void addClient( client );
	void removeClient( client );
	void changeModes( int );

	std::vector<client>& getClients( void );
};

#endif
