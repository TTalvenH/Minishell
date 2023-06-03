/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:02:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/01 18:18:10 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_flag(char *flag)
{
	int	i;

	i = 1;
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (1);
		i++;
	}
	return (0);
}

int	echo(char **args)
{
	int	linebreak;
	int	i;

	linebreak = 0;
	i = 0;
	if (args[i] && args[i][0] == '-')
	{
		if (!check_flag(args[i]))
		{
			linebreak = 1;
			i++;
		}
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (!linebreak)
		write(1, "\n", 1);
	return (0);
}
