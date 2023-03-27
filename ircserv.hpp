/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ircserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccambium <ccambium@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 06:34:50 by ccambium          #+#    #+#             */
/*   Updated: 2023/03/22 06:45:03 by ccambium         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCSERV_HPP
#define IRCSERV_HPP

#include <iostream>
#include "client.hpp"
#include "sys/epoll.h"

class ircserv {
   private:
	ircserv();
	static int         _port;
	static bool        _failed;
	static std::string _password;
	static client      _clients[1024];
	static int         _pollfd;
	static int         _tcp6_socket;

	static void accept_client( epoll_event& );
	static void process_events( epoll_event& );

   public:
	static void initialisation( char* pass, char* port );
	static void start( void );

	static bool failed( void );

	static int getPollfd( void );
};

#endif
