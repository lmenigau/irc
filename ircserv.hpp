/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccambium <ccambium@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 06:34:50 by ccambium          #+#    #+#             */
/*   Updated: 2023/05/02 16:32:38 by odessein         ###   ########.fr       */
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
#include "typedef.hpp"

class ircserv {
  
   private:
   	static t_client_array _clients;
	ircserv();
	static int           _port;
	static bool          _failed;
	static std::string   _password;
	static t_map_channel _channels;
	static int           _pollfd;
	static int           _tcp6_socket;
	static std::string   _servername;

	static void accept_client( epoll_event& );
	static void process_events( epoll_event& );

   public:
	static void initialisation( char* pass, char* port );
	static void start( void );
	//  static std::vector<Client>::iterator getClientFromVector(int fd);

	static bool failed( void );

	static int            getPollfd( void );
	static std::string    getPassword( void );
	static t_map_channel& getChannels( void );
	static std::string    getServername( void );
	static t_client_array &getClients( void );

	static void addChannel( std::string&, Client& );
	static void removeChannel( std::string );

	static void removeClient( Client& c );
};

#endif
