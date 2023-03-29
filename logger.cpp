#include <iostream>
#include "utils.hpp"

#define COLOR_DEBUG "\e[92m"
#define COLOR_INFO "\e[96m"
#define COLOR_ERROR "\e[91m"
#define COLOR_FATAL "\e[31m"
#define COLOR_WARNING "\e[33m"
#define RESET "\e[0m"

void _debug( std::string f, va_list va_args ) {
	std::cout << COLOR_DEBUG << "DEBUG   | " << format( f, va_args ) << RESET
	          << '\n';
}

void _info( std::string f, va_list va_args ) {
	std::cout << COLOR_INFO << "INFO    | " << format( f, va_args ) << RESET
	          << '\n';
}

void _error( std::string f, va_list va_args ) {
	std::cout << COLOR_ERROR << "ERROR   | " << format( f, va_args ) << RESET
	          << '\n';
}

void _fatal( std::string f, va_list va_args ) {
	std::cout << COLOR_FATAL << "FATAL   | " << format( f, va_args ) << RESET
	          << '\n';
}

void _warning( std::string f, va_list va_args ) {
	std::cout << COLOR_WARNING << "WARNING | " << format( f, va_args ) << RESET
	          << '\n';
}

void logger( std::string level, std::string f, ... ) {
	void ( *loggers[5] )( std::string, va_list ) = { _debug, _info, _error,
	                                                 _fatal, _warning };
	std::string log_level[5] = { "DEBUG", "INFO", "ERROR", "FATAL", "WARNING" };
	size_t      i            = 0;
	va_list     va_args;
	va_start( va_args, f );
	while ( i < 5 ) {
		if ( !level.compare( log_level[i] ) ) {
			loggers[i]( f, va_args );
			break;
		}
		i++;
	}
	if ( i == 5 )
		_warning( format( "%s is not a valid level !\n", level.c_str() ),
		          va_args );
	va_end( va_args );
}
