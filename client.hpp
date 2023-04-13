#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <map>
#include <string>
/*
struct client {
	bool        ispolled;
	bool        isregistered;
	int         fd;
	int         start;
	int         end;
	std::string buf;
	std::string nick;
	std::string out;
	void        reply( std::string const str );
};
*/

class Client {
	public :
		bool		ispolled;
		bool		isregistered;
		int			fd;
		int			start;
		int			end;
		std::string	buf;
		std::string	nick;
		std::string	out;

		Client();
		~Client();
		void reply(std::string const &str);
};

//std::map<std::string, Client> users;

#endif
