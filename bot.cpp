#include "Bot.hpp"

#define MAX_PORT 65535

std::string	Bot::_addr = "";
int			Bot::_port = 0;
std::string	Bot::_pwd = "";
std::string	Bot::_name = "";
int			Bot::_socket = 0;
int			Bot::_fd = 0;

static bool check_num(char *port)
{
	int i = 0;

	while (port[i])
	{
		if (port[i] < 48 || port[i] > 57)
			return (false);
		i++;
	}
	return (true);
}

static void send_msg(std::string msg, int socket)
{
	msg += "\r\n";
	if (!send(socket, msg.c_str(), msg.size(), MSG_DONTWAIT))
		std::cerr << "cannot send msg" << std::endl;
}

bool Bot::connectToServer(char **av) {

    struct addrinfo hints;
    struct addrinfo* result;
    struct addrinfo* ptr;
    int sockfd = -1;
	int	status = 0;

	if (strlen( av[2] ) > 5)
		return (false);
	_port = atoi(av[2]);
	if (_port > MAX_PORT || _port <= 0 || !check_num(av[2]))
		return (false);
	if (strlen(av[3]) == 0)
		return (false);
	_pwd = av[3];

	_addr = av[1];
    // Set up hints
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;   // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    status = getaddrinfo(_addr.c_str(), av[2], &hints, &result);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return 1;
    }
    //for (ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        // Create socket
        _socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (_socket == -1) 
            std::cerr << "socket creation error" << std::endl;
        // Connect to the server
        if (connect(_socket, result->ai_addr, result->ai_addrlen) == 0) {
            // Connection successful
		    freeaddrinfo(result);
			return (true);
        } else {
            // Connection failed, close the socket
            close(_socket);
            _socket = -1;
        }
	return (false);
}

void	Bot::intro(void)
{
	fd_set	fds;
	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	send_msg("PASS " + _pwd, _socket);
	send_msg("NICK bot", _socket);
	send_msg("USER bot", _socket);

	FD_ZERO(&fds);
	FD_SET(_socket, &fds);

	char str[4096];
	if (select(_socket + 1 , &fds, NULL, NULL, &tv) > 0)
	{
		if (recv(_socket, &str, sizeof(str), 0) == -1)
			std::cerr << "OUPSS" << std::endl;
		std::cout << str << " " << std::endl;
	}
}



void	Bot::end(void)
{
	close(_socket);
}
