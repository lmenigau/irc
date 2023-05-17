#include "messageBuilder.hpp"

MessageBuilder::MessageBuilder() {}

MessageBuilder::~MessageBuilder() {}

MessageBuilder::MessageBuilder( const MessageBuilder& src ) { *this = src; }

std::string MessageBuilder::getBuff() const { return ( buff ); }

void MessageBuilder::clear() { buff.clear(); }

size_t MessageBuilder::size() const { return ( buff.size() ); }

MessageBuilder& MessageBuilder::operator=( const MessageBuilder& rhs ) {
	if ( this != &rhs ) {
		buff = rhs.buff;
	}
	return ( *this );
}

MessageBuilder& MessageBuilder::operator=( const std::string& rhs ) {
	buff = rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator+=( const std::string& rhs ) {
	buff += rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator+=( const char rhs ) {
	buff += rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator+( const std::string& rhs ) {
	buff += rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator+( const char rhs ) {
	buff += rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator+( const MessageBuilder& rhs ) {
	buff += rhs.buff;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator<<( const std::string& rhs ) {
	buff += rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator<<( const char rhs ) {
	buff += rhs;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator<<( const MessageBuilder& rhs ) {
	buff += rhs.buff;
	return ( *this );
}

MessageBuilder& MessageBuilder::operator<<( const int rhs ) {
	buff += SSTR( rhs );
	return ( *this );
}

MessageBuilder& MessageBuilder::operator<<( const unsigned int rhs ) {
	buff += SSTR( rhs );
	return ( *this );
}

std::ostream& operator<<( std::ostream& os, MessageBuilder& m ) {
	os << m.getBuff();
	return ( os );
}

MessageBuilder::operator std::string() { return ( buff ); }
