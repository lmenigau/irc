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

class	ircserv{
	private:
		ircserv();
		static int	_port;
		static bool	_failed;
		static bool _password;

	public:
		static void	initialisation(char * pass, char * port);
		static void	start( void );

		static bool	failed( void );
};
