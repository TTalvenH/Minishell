/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:44:31 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/01 18:25:19 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_all_envs(t_new_line *got_line, int instruction)
{
	int			i;
	static char	*holder = NULL;

	i = 0;
	while (got_line->our_environ[i] && instruction == 1)
	{
		if (got_line->our_environ[i] && got_line->our_environ[i][0] == '?')
		{
			i++;
			continue ;
		}
		ft_printf("%s\n", got_line->our_environ[i++]);
	}
	while (got_line->our_environ[i] && instruction == 0)
	{
		holder = ft_strchr(got_line->our_environ[i], '=');
		if (got_line->our_environ[i] && holder[0] && (got_line->our_environ[i][0] == '?' || holder[1]))
		{
			i++;
			continue ;
		}
		ft_printf("%s\n", got_line->our_environ[i++]);
	}
	return (EXIT_SUCCESS);
}
