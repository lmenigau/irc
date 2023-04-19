#ifndef UTILS_HPP
#define UTILS_HPP

#include <algorithm>
#include <cstdarg>
#include <string>

std::string format( std::string format, ... );
std::string format( std::string format, va_list va_args );
void        logger( std::string level, std::string format, ... );
std::string trim( const std::string& s );
void        remove_backslash_r( std::string& c );

#endif
