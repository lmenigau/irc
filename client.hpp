#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <map>
#include <string>

class Client {
   public:
	int         start;
	int         end;
	std::string buf;
	std::string out;

	Client( int );
	~Client( void );
	Client( void );

	// methods

	void reply( std::string const& str );
	// getters

	int         getFd( void );
	std::string getCap( void );
	std::string getNick( void );
	std::string getUser( void );
	std::string getModes( void );

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

	// state accessors ?

	bool isPolled( void );
	bool hasGivenNick( void );
	bool hasGivenUser( void );
	bool hasGivenPassword( void );
	bool isRegistered( void );

   private:
	int         _fd;
	std::string _modes;
	std::string _cap;
	std::string _nick;
	std::string _user;
	bool        _hasGivenNick;
	bool        _hasGivenUser;
	bool        _hasGivenPassword;
	bool        _isRegistered;
	bool        _isPolled;
};

// std::map<std::string, Client> users;

#endif
