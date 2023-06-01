/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:44:31 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 11:52:54 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_all_envs(t_new_line *got_line)
{
	int	i;

	i = 0;
	while (got_line->our_environ[i])
	{
		if (got_line->our_environ[i] && got_line->our_environ[i][0] == '?')
		{
			i++;
			continue ;
		}
		ft_printf("%s\n", got_line->our_environ[i++]);
	}
		
	return (EXIT_SUCCESS);
}
