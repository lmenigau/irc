#ifndef UTILS_HPP
#define UTILS_HPP

#include <limits.h>
#include <algorithm>
#include <cstdarg>
#include <string>
#include "channel.hpp"
#include "client.hpp"
#include "typedef.hpp"
#include "messageBuilder.hpp"

typedef enum err { MODE, FLAG } t_err_type;

typedef enum type { CHANNEL, USER } t_type;

void        logger( std::string level, std::string string );
void        logger( std::string level, MessageBuilder &mb );
bool        isUser( std::string str );
bool        isChannel( std::string str );
std::string trim( const std::string& s );
void        remove_backslash_r( std::string& c );
Client*     find_client( std::string );
Channel*    find_channel( std::string );
void        welcome( Client* );
void        close_client( Client& client );

template <typename T>
void forEach( std::vector<T>& vec, void ( *f )( T& ) ) {
	typename std::vector<T>::iterator it = vec.begin();
	while ( it != vec.end() ) {
		f( *it );
		it++;
	}
}
std::string getTarget( size_t& pos, std::string str );
bool        isValidPositiveNumber( std::string args );
bool		check_num(char *port);

#endif
