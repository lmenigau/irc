#ifndef TYPEDEF_HPP
# define TYPEDEF_HPP

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

class Channel;
class Client;

typedef std::map<int, Client> t_map_client;
typedef std::map<std::string, Channel> t_map_channel;
typedef std::vector<Client *> t_vector_client_ptr;

#endif
