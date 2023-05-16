#ifndef MESSAGE_BUILDER
# define MESSAGE_BUILDER

#include <string>
#include <iostream>
#include <sstream>

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

class MessageBuilder{
	private:
		std::string buff;

	public:
		MessageBuilder();
		MessageBuilder(const MessageBuilder &);
		~MessageBuilder();

		std::string getBuff() const;
		void clear();
		size_t size() const;

		MessageBuilder& operator=(const MessageBuilder &);
		MessageBuilder& operator=(const std::string &);
		MessageBuilder& operator+=(const std::string &);
		MessageBuilder& operator+=(const char);
		MessageBuilder& operator+(const std::string &);
		MessageBuilder& operator+(const char);
		MessageBuilder& operator+(const MessageBuilder &);
		MessageBuilder& operator<<(const std::string &);
		MessageBuilder& operator<<(const char);
		MessageBuilder& operator<<(const MessageBuilder &);
		MessageBuilder& operator<<(const int);
		MessageBuilder& operator<<(const unsigned int);
};

std::ostream &operator<<(std::ostream &o, MessageBuilder &m);
#endif
