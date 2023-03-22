/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccambium <ccambium@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 10:22:31 by ccambium          #+#    #+#             */
/*   Updated: 2023/03/22 15:03:36 by ccambium         ###   ########.fr       */
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

std::list<std::string> *parse(std::string input)
{
	size_t		len = count_word(input);
	std::list<std::string> *ret_v = new std::list<std::string>;
	size_t		cursor = 0;
	for (size_t i = 0; i < len;i++)
	{
		if (i != len - 1)
			ret_v->push_back(input.substr(cursor, input.find(' ', cursor + 1) - cursor));
		else
        {
            if (input[cursor] != ':')
			    ret_v->push_back(input.substr(cursor, std::string::npos));
            else
                ret_v->push_back(input.substr(cursor + 1, std::string::npos));
        }
        cursor += input.find(' ', cursor + 1) - cursor + 1;
        if (cursor > input.length())
            break ;
	}
	return ret_v;
}
