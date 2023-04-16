#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdarg>
#include <string>

std::string format( std::string format, ... );
std::string format( std::string format, va_list va_args );
void        logger( std::string level, std::string format, ... );
bool		isUser( std::string str );
bool		isChannel( std::string str );

#endif
