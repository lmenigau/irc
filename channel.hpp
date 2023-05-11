#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include "client.hpp"
#include "typedef.hpp"

typedef enum mode_operation {
	ADD,
	SUB,
	NONE
}	t_ope;

class Channel {
   private:
	t_vector_client_ptr             _clients;
	std::string                    _modes;
	std::string                    _name;
	std::string                    _topic;
	std::string                    _password;
	t_vector_client_ptr           _ops;
	t_vector_client_ptr				    _banned;
	std::string                  _key;
	bool                         _invite_only;
	bool												 _topic_op;
	int                            _limit;

	void m_topic(Client &c, std::string args, t_ope operation);
	void m_ban(Client &c, std::string args, t_ope operation);
	void m_operator(Client &c, std::string args, t_ope operation);
	void m_invite(Client &c, std::string args, t_ope operation);
	void m_key(Client &c, std::string args, t_ope operation);
	void m_limit(Client &c, std::string args, t_ope operation);

   public:
	Channel();
	Channel( std::string );
	Channel( Client& creator, const std::string& name );
	Channel( const Channel &a);
	~Channel();
	void addClient( Client & );
	void removeClient( Client& );
	void changeModes( int );
  bool isOps(Client &c);
bool	findClients( const std::string &nick );
bool	topicSet(void);
void	setTopic(const std::string &topic);
std::string getTopic(void);

	std::string addModes( std::string );
	std::string removeModes( std::string );
	t_vector_client_ptr &getOps(void);
	std::string getModes( void );
	void handleModes( Client &c, std::string modes, std::string args);


	void                            setModes( std::string );
	t_vector_client_ptr& getClients( void );
	void                            sendAll( std::string msg );
	void                            sendAll( std::string msg, Client& );
};

#endif
