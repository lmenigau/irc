#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "client.hpp"

class Channel {
   private:
	std::map<std::string, Client*> _clients;
	std::string                    _modes;
	std::string                    _name;
	std::string                    _topic;
	std::string                    _password;
	std::vector<Client*>           _ops;
	std::vector<Client*>           _halfops;
	std::vector<Client*>           _voiced;
	std::vector<Client*>           _founder;
	std::vector<std::string>       _banned;
	std::string                    _key;
	//	int                            _limit;

   public:
	Channel();
	Channel( std::string );
	Channel( Client& creator, const std::string& name );
	~Channel();
	void addClient( Client& );
	void removeClient( Client& );
	void changeModes( int );

	std::string addModes( std::string );
	std::string removeModes( std::string );

	std::string getModes( void );

	void                            setModes( std::string );
	std::map<std::string, Client*>& getClients( void );
	void							sendAll(std::string msg);
	void							sendAll(std::string msg, Client &);

};

#endif
