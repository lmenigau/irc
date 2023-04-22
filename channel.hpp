#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "client.hpp"

class Channel {
   private:
	t_vector_client_ref	             _clients;
	std::string                    _modes;
	std::string                    _name;
	std::string                    _topic;
	std::string                    _password;
	t_vector_client_ref	          _ops;
	t_vector_client_ref           _halfops;
	t_vector_client_ref          _voiced;
	t_vector_client_ref          _founder;
	std::vector<std::string>       _banned;
	std::string                    _key;
//	int                            _limit;


  public :
  Channel();
	Channel( std::string );
	Channel( Client& creator, const std::string& name );
	~Channel();
	void addClient( Client* );
	void removeClient( Client& );
	void changeModes( int );

	std::string addModes( std::string );
	std::string removeModes( std::string );

	std::string getModes( void );

	void                            setModes( std::string );
	t_vector_client_ref& getClients( void );
};

#endif
