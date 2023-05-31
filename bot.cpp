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
	fd_set	fds;
	struct timeval tv;

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(socket, &fds);

	msg += "\r\n";
	if (select(socket + 1 , NULL, &fds, NULL, &tv) > 0)
	{
		if (!send(socket, msg.c_str(), msg.size(), MSG_DONTWAIT))
			std::cerr << "cannot send msg" << std::endl;
	}
	FD_CLR(socket, &fds);
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

static bool check(std::string word)
{
	std::ifstream word_list;
	std::string word_in_list;

	word_list.open("word_list");
	if (word_list.bad())
		std::cerr << "Failed opening word_list" << std::endl;
	while (getline(word_list, word_in_list))
		if (word == word_in_list)
			return (true);
	word_list.close();
	return (false);
}

void Bot::ban_user(std::string reply)
{
	size_t pos = reply.find("!");
	if (pos != std::string::npos)
	{
		std::string nick = reply.substr(1, pos - 1);
		std::string chan_name = reply.substr(reply.find("#"), reply.find(" ", reply.find("#")) - reply.find("#"));
	 	send_msg("MODE " + chan_name + " -o " + nick, _socket);
	 	send_msg("MODE " + chan_name + " +b " + nick, _socket);
	 	send_msg("NOTICE " + nick + " :You got banned because you said some injury on a channel of our IRC server", _socket);
	}
}

static bool injury(std::string reply)
{
	std::string msg;
	std::string word;

	size_t pos = reply.find(":", 1);

	if (pos != std::string::npos)
		msg = reply.substr(pos + 1);
	else
		return (false);
	pos = 0;
	while (pos != std::string::npos)
	{
		size_t buff_pos = pos;
		pos = msg.find(" ", buff_pos);
		if (pos == std::string::npos)
			word = msg.substr(buff_pos, (msg.find("\r") - buff_pos));
		else
		{
			pos += 1;
			word = msg.substr(buff_pos, pos - buff_pos - 1);
		}
		if (check(word))
			return (true);
	}
	return (false);
}

void	Bot::intro(void)
{
	fd_set	fds;
	struct timeval tv;
	char str[4096];

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	FD_ZERO(&fds);
	FD_SET(_socket, &fds);

	send_msg("PASS " + _pwd, _socket);
	send_msg("NICK bot_irc", _socket);
	send_msg("USER bot", _socket);

	if (select(_socket + 1 , &fds, NULL, NULL, &tv) > 0)
	{
		if (recv(_socket, &str, sizeof(str), 0) == -1)
			std::cerr << "Failed to receive data" << std::endl;
	}
	FD_CLR(_socket, &fds);
}

bool Bot::handleReply(char str[4096])
{
	std::string reply = str;
	size_t	pos = 0;
	size_t	buff_pos;

	while (pos != std::string::npos)
	{
		buff_pos = pos;
		pos = reply.find("\n", buff_pos + 1);
		if (reply.size() == 0)
			return (false);
		if (reply.find("PRIVMSG") == std::string::npos)
			continue ;
		if (injury(reply.substr(buff_pos, pos)))
			ban_user(reply.substr(buff_pos, pos));
	}
	return (true);
}

void	Bot::loop(void)
{
	fd_set	fds;
	struct timeval tv;
	char str[4096];

	tv.tv_sec = 1;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(_socket, &fds);
	while (1)
	{
		if (select(_socket + 1 , &fds, NULL, NULL, &tv) > 0)
		{
			if (recv(_socket, &str, sizeof(str), 0) == -1)
				std::cerr << "Failed to receive data" << std::endl;
			else
			{
				if (!handleReply(str))
					break ;
				bzero(str, 4096);
				continue;
			}
		}
		FD_CLR(_socket, &fds);
		FD_ZERO(&fds);
		FD_SET(_socket, &fds);
	}
	FD_CLR(_socket, &fds);
}

void	Bot::end(void)
{
	close(_socket);
}
