/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccambium <ccambium@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 06:34:50 by ccambium          #+#    #+#             */
/*   Updated: 2023/04/13 22:45:47 by ccambium         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include "channel.hpp"
#include "client.hpp"
#include "sys/epoll.h"

class ircserv {
   private:
	ircserv();
	static int                            _port;
	static bool                           _failed;
	static std::string                    _password;
	static std::map<std::string, Channel> _channels;
	static int                            _pollfd;
	static int                            _tcp6_socket;

	static void accept_client( epoll_event& );
	static void process_events( epoll_event& );

   public:
	static void initialisation( char* pass, char* port );
	static void start( void );

	static bool failed( void );

	static int                             getPollfd( void );
	static std::vector<Client*>            _clients;
	static std::string                     getPassword( void );
	static std::map<std::string, Channel>& getChannels( void );

	static void addChannel( std::string&, Client& );
	static void removeChannel( std::string );
};

#endif
