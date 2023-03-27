#ifndef CLIENT_HPP
#define CLIENT_HPP
#include <map>
#include <string>

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

extern std::map<std::string, client> users;
#endif
