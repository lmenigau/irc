#ifndef TYPEDEF_HPP
#define TYPEDEF_HPP

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <vector>

class Channel;
class Client;

typedef std::vector<Client>            t_client_array;
typedef std::map<std::string, Client>  t_map_string_client;
typedef std::map<std::string, Client*> t_map_string_client_ref;
typedef std::map<std::string, Channel> t_map_channel;
typedef std::vector<Client*>           t_vector_client_ptr;

#endif
