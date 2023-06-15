#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <netdb.h>
#include <sys/select.h>
#include <fstream>

class Bot {
	private:
		static std::string	_addr;
		static int			_port;
		static std::string	_pwd;
		static std::string	_name;
		static int			_socket;
		static int			_fd;
	public:
		static bool connectToServer(char **av);
		static void	intro(void);
		static void	loop(void);
		static void end(void);
		static void ban_user(std::string reply);
		static bool handleReply(char str[4096]);
};

void logger( std::string level, std::string string );
