#include "utils.hpp"
#include "ircserv.hpp"
#include <map>
#include <list>

//Modes string should not overexceed 3 letter
//Some modes are not combinables : like k is ok with i / t but not o or l
// o is ok with i / t not k or l
// Conclusion : l k o are not possible together i and t are okay with all

// Ban whould be alone / Same for voice, maybe ok with i / t but lazy ..

//its possible to have +o-v etc .. 3 instruction max

static bool is_valid_user_mode( char mode ) {
	return ( mode == 'i' || mode == 'o' || mode == 'B');
}

static bool is_valid_channel_mode( char mode ) {
	return ( mode == 'i' || mode == 't' || mode == 'b' || mode == 'k' || mode == 'l' || mode == 'o');
}

static bool search_mode(std::string str, std::string letter, bool &res)
{
	int		pos;

	if (str[0] != '+' && str[0] != 'b' && str[0] != '-')
		return (false);
	if (str.find(letter) != std::string::npos)
	{
		if (res)
			return (false);
		res = true;
		pos = str.find(letter);
		if (str.find(letter, pos + 1) != std::string::npos)
			return (false);
	}
	return (true);
}

static bool check_char_modes (std::string str, t_type type, t_err_type &err_type)
{
	for (std::string::iterator it = str.begin() + 1; it != str.end(); it++)
	{
		if ((type == USER && !is_valid_user_mode(*it)) || (type == CHANNEL && !is_valid_channel_mode(*it)))
		{
			err_type = FLAG;
			return (false);
		}
	}
	return (true);
}

static bool check_user_modes( std::vector<std::string> modes, t_err_type &err_type) {
	bool	b = false;
	bool	o = false;
	bool	i = false;

	for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); it++) {
		if (!check_char_modes(*it, USER, err_type))
			return (false);
		if (!search_mode(*it, "b", b))
			return (false);
		if (!search_mode(*it, "i", i))
			return (false);
		if (!search_mode(*it, "o", o))
			return (false);
	}
	if (b && o)
		return (false);
	if (!b && !o && !i)
	{
		err_type = FLAG;
		return (false);
	}
	return (true);
}

static bool check_channel_modes( std::vector<std::string> modes, t_err_type &err_type ) {
	bool i = false;
	bool t = false;
	bool b = false;
	bool k = false;
	bool l = false;
	bool o = false;

	if (modes.front().find("b") != std::string::npos && modes.front()[0] == 'b' && modes.size() > 1)
		return (false);

	for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); it++) {
		if (!check_char_modes(*it, CHANNEL, err_type))
			return (false);
		if (!search_mode(*it, "t", t))
			return (false);
		if (!search_mode(*it, "b", b))
			return (false);
		if (!search_mode(*it, "o", o))
			return (false);
		if (!search_mode(*it, "i", i))
			return (false);
		if (!search_mode(*it, "k", k))
			return (false);
		if (!search_mode(*it, "l", l))
			return (false);
	}
	if (i && (b || l))
		return (false);
	//We are not handling special case with +o
	if (o && (t || b || i || k || l))
		return (false);
	if (b && (k || l || t))
		return (false);
	if (t && l)
		return (false);
	if (!i && !t && !b && !k && !l && !o)
	{
		err_type = FLAG;
		return (false);
	}
	return (true);
}

void user_mode( Client&     c,
                std::list <std::string> *args,
                std::vector<std::string> modes, std::string target) {
	if ( target != c.getNick() ) {
		c.reply(
		    ":ircserv.localhost 502 :Cant change mode for other users\r\n" );
		return;
	}
	(void) args;
	for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); it++)
	{
		if ((*it)[0] == '+')
			c.addModes( *it );
		else if ((*it)[0] == '-')
			c.removeModes( *it );
	c.reply( format( ":ircserv.localhost 221 %s %s\r\n", c.getUser().c_str(),
	                (*it).c_str() ) );
	logger( "DEBUG", "user %s has now mode %s", c.getUser().c_str(),
	        (*it).c_str() );
	}
}

//The Client should be Operator to use modes

void channel_mode( Client&     c,
                   std::list <std::string> *args,
                   std::vector<std::string> modes, std::string target_str) {

	std::string target;
	size_t	i = 0;

	while (i != args->front().size())
	{
			target = getTarget(i, target_str);
			if (target.empty())
				continue ;
		if ( modes.empty() ) {
			c.reply( format( ":ircserv.localhost 324 %s %s +\r\n",
		                 	c.getUser().c_str(), target.c_str() ) );
			return;
		}
		try {
			ircserv::getChannels().at( target );
			//else
				//channel->callModes(modes)
		} catch ( std::exception& e ) {
			c.reply( format( ":ircserv.localhost 403 %s :No such channel\r\n",
		                 	target.c_str() ) );
			return ;
		}
			Channel *channel = find_channel(target);
			for (std::vector<std::string>::iterator it = modes.begin(); it != modes.end(); it++)
			{
				//Maybe a method of channel like handle_mode, taking the string of mode + args->back() which could parse string of mode and call function depending on it
				channel->handleModes(c, *it, args->back());
				logger( "DEBUG", "user %s has now mode %s", c.getUser().c_str(),
			        	(*it).c_str() );
			}
	}
}

void	fill_from_string(std::string str, std::vector<std::string> &modes)
{
	int			i = 0;
	int			pos = 0;
	bool		start = true;

	for (std::string::iterator it = str.begin(); it != str.end(); it++)
	{
		if (*it == '-' || *it == '+')
		{
			if (!start)
			{
				modes.push_back(str.substr(pos, i - pos));
				pos = i;
			}
			else
				start = false;
		}
		i++;
	}
	modes.push_back(str.substr(pos, i - pos));
}

void mode( std::list<std::string>* args, Client& c ) {
	if ( args->empty() || args->size() == 1 )
		return;
	std::vector<std::string> modes;
	std::string target = args->front();
	t_err_type err_type = MODE;
	args->pop_front();

	fill_from_string(args->front(), modes);
	if ( isUser( target ) ) {
		if (!check_user_modes( modes, err_type ))
		{
			if (err_type == MODE)
				c.reply( format(":ircserv.localhost 472 %s :is unknow mode char to me\r\n", target.c_str()));
			else if (err_type == FLAG)
				c.reply(":ircserv.localhost 501 :Unknown MODE flag\r\n");
			return ;
		}
		user_mode( c, args, modes, target);
	}
	if ( isChannel( target ) ) {
		if (!check_channel_modes(modes, err_type))
		{
			if (err_type == MODE)
				c.reply( format(":ircserv.localhost 472 %s :is unknow mode char to me\r\n", target.c_str()));
			else if (err_type == FLAG)
				c.reply(":ircserv.localhost 501 :Unknown MODE flag\r\n");
			return ;
		}
		channel_mode( c, args, modes, target);
	}
}
