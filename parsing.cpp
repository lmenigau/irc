/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccambium <ccambium@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 10:22:31 by ccambium          #+#    #+#             */
/*   Updated: 2023/03/22 11:07:52 by ccambium         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.hpp"

static size_t	count_word(std::string &input)
{
	size_t	ret_v = 1;
	for (size_t i = 0; i < input.length(); i++)
	{
		if (input[i] == ' ')
        {
            while (input[i] == ' ')
               i++;
			ret_v ++;
        }
		if (input[i] == ':')
			break ;
	}
	return ret_v;
}

std::string *parse(std::string input)
{
	size_t		len = count_word(input);
	std::string *ret_v = new std::string[len];
	size_t		cursor = 0;
	for (size_t i = 0; i < len;)
	{
		if (i != len - 1)
			ret_v[i].assign(input.substr(cursor, input.find(' ', cursor + 1)));
		else
			ret_v[i].assign(input.substr(cursor, std::string::npos));
	}
	return ret_v;
}
