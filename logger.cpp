#include <iostream>
#include "messageBuilder.hpp"

#define COLOR_DEBUG "\e[92m"
#define COLOR_INFO "\e[96m"
#define COLOR_ERROR "\e[91m"
#define COLOR_FATAL "\e[31m"
#define COLOR_WARNING "\e[33m"
#define RESET "\e[0m"

void _debug( std::string string ) {
	std::cout << COLOR_DEBUG << "DEBUG   | " << string << RESET << '\n';
}

void _info( std::string string ) {
	std::cout << COLOR_INFO << "INFO    | " << string << RESET << '\n';
}

void _error( std::string string ) {
	std::cout << COLOR_ERROR << "ERROR   | " << string << RESET << '\n';
}

void _fatal( std::string string ) {
	std::cout << COLOR_FATAL << "FATAL   | " << string << RESET << '\n';
}

void _warning( std::string string ) {
	std::cout << COLOR_WARNING << "WARNING | " << string << RESET << '\n';
}

void logger( std::string level, std::string string ) {
	void ( *loggers[5] )( std::string ) = { _debug, _info, _error, _fatal,
	                                        _warning };
	std::string log_level[5] = { "DEBUG", "INFO", "ERROR", "FATAL", "WARNING" };
	size_t      i            = 0;
	MessageBuilder mb;
	while ( i < 5 ) {
		if ( !level.compare( log_level[i] ) ) {
			loggers[i]( string );
			return;
		}
		i++;
	}
	_warning( mb << level << " is not a valid level !" );
}

void logger( std::string level, MessageBuilder mb ) {
	logger( level, mb.getBuff() );
	mb.clear();
}
