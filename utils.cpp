#include <cstdarg>
#include <sstream>
#include <string>

std::string format( std::string format, ... ) {
	va_list va_args;
	size_t  i = 0;

	va_start( va_args, format );
	std::string s( format );
	while ( s.find( '%', i ) != std::string::npos ) {
		i = s.find( '%', i );
		if ( s[i + 1] == 's' )
			s.replace( i, 2, (char*) va_arg( va_args, char* ) );
		else if ( s[i + 1] == 'c' ) {
			s.replace( i, 1, 1, ' ' );
			s.replace( i + 1, 1, 1, (char) va_arg( va_args, int ) );
		} else if ( s[i + 1] == 'd' ) {
			std::stringstream ss;
			ss << (int) va_arg( va_args, int );
			s.replace( i, 2, ss.str() );
		} else
			break;
	}
	va_end( va_args );
	return ( s );
}

std::string format( std::string format, va_list va_args ) {
	size_t i = 0;

	std::string s = format;
	while ( s.find( '%', i ) != std::string::npos ) {
		i = s.find( '%', i );
		if ( i + 1 == 's' )
			s.replace( i, 2, (char*) va_arg( va_args, char* ) );
		else if ( i + 1 == 'c' ) {
			s.replace( i, 1, 1, ' ' );
			s.replace( i + 1, 1, 1, (char) va_arg( va_args, int ) );
		} else if ( i + 1 == 'd' ) {
			std::stringstream ss;
			ss << (int) va_arg( va_args, int );
			s.replace( i, 2, ss.str() );
		} else
			break;
	}
	return ( s );
}
