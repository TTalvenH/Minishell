/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:44:31 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/12 17:29:53 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_all_envs(t_new_line *got_line, int instruction)
{
	int			i;
	int			equals;

	i = 0;
	while (got_line->our_environ[i])
	{
		equals = find_index_of(got_line->our_environ[i], '=');
		if (got_line->our_environ[i][0] != '?' && instruction == 0
				&& got_line->our_environ[i][++equals] == '\0')
			ft_printf("%s\n", got_line->our_environ[i++]);
		else if (got_line->our_environ[i][0] != '?' && instruction == 1
				&& got_line->our_environ[i][++equals] != '\0')
			ft_printf("%s\n", got_line->our_environ[i++]);
		else
			i++;
	}
	return (EXIT_SUCCESS);
}

int	find_index_of(const char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (0);
}
