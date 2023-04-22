#ifndef TYPEDEF_HPP
# define TYPEDEF_HPP

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"

typedef std::map<int, Client> t_map_int_client:
typedef std::map<std::string, Client> t_map_string_client;
typedef std::map<std::string, Channel> t_map_channel;
typedef std::vector<Client *> t_vector_client_ref;

#endif
