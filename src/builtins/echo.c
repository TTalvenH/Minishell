/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 11:02:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 11:55:45 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	echo(char **args, char **environments)
{
	int	linebreak;
	int	i;

	linebreak = 0;
	i = 0;
	(void)environments;
	if (args[i][0] && args[i][0] == '-' && args[i][1]
		&& args[i][0] == 'n' && args[i][2] && args[i][0] == '\0')
	{
		linebreak = 1;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		i++;
	}
	if (!linebreak)
		write(1, "\n", 1);
	exit(EXIT_SUCCESS);
}
