#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <netinet/in.h>
#include <sys/socket.h>
#include <map>
#include <string>
#include "messageBuilder.hpp"

class Client {
   public:
	int         start;
	int         end;
	std::string buf;
	std::string out;

	Client( int );
	Client( int fd, sockaddr_in6& addr );
	Client( const Client& a );
	~Client( void );
	Client( void );

	// methods

	void        reply( std::string const& str );
	void        reply( MessageBuilder &mb );
	std::string addModes( std::string );
	std::string removeModes( std::string );
	// getters

	int         getFd( void );
	std::string getCap( void );
	std::string getNick( void );
	std::string getUser( void );
	std::string getRealUser( void );
	std::string getHostname( void );
	std::string getModes( void );
	bool        isServerOp( void );
	void        setOp( void );
	void        unsetOp( void );
	void        unsetInvisible( void );
	void        unsetBot( void );

	// setters

	void setCap( std::string );
	void setNick( std::string );
	void setHasGivenPassword( bool );
	void setHasGivenUser( bool );
	void setHasGivenNick( bool );
	void setIsPolled( bool );
	void setFd( int );
	void setUser( std::string );
	void setModes( std::string );
	void setRealUser( std::string );
	void setHostname( struct sockaddr_in6& addr );
	void setHasBeenWelcomed( bool );
	void setIsOper( bool );
	void setInvisible( void );
	void setBot( void );
	void setDestroy( void );

	// state accessors ?

	bool isPolled( void );
	bool isBot( void );
	bool isInvisble( void );
	bool hasGivenNick( void );
	bool hasGivenUser( void );
	bool hasGivenPassword( void );
	bool isRegistered( void );
	bool hasBeenWelcomed( void );
	bool isOper( void );
	bool toDestroy( void );

   private:
	int         _fd;
	bool        _server_op;
	std::string _modes;
	std::string _cap;
	std::string _nick;
	std::string _user;
	std::string _realuser;
	std::string _hostname;
	bool        _hasBeenWelcomed;
	bool        _hasGivenNick;
	bool        _hasGivenUser;
	bool        _hasGivenPassword;
	bool        _isPolled;
	bool        _isBot;
	bool        _isOper;
	bool        _invisible;
	bool		_destroy;
};

// std::map<std::string, Client> users;

#endif
