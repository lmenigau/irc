/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccambium <ccambium@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 06:40:00 by ccambium          #+#    #+#             */
/*   Updated: 2023/03/22 11:13:44 by ccambium         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_HPP
#define IRC_HPP

#include <asm-generic/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <ostream>
#include <queue>
#include <sstream>
#include "parsing.hpp"
struct client {
    bool ispolled;
    bool isregistered;
    int fd;
    int start;
    int end;
    std::string buf;
    std::string nick;
    std::string out;
    void reply(std::string const str);
};

#endif
