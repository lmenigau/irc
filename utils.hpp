#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdarg>
#include <string>

std::string format( std::string format, ... );
std::string format( std::string format, va_list va_args );

#endif
