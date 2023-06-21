/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:39:10 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/05/31 20:08:24 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "minishell.h"

static int	check_argument(char *argument)
{
	int	i;

	i = 0;
	if (!argument[i])
		return (1);
	if (argument[i] == '-' || argument[i] == '+' )
		i++;
	while (argument[i])
	{
		if (!ft_isdigit(argument[i]))
			return (1);
		i++;
	}
	return (0);
}

int	exit_builtin(char **args, t_pipe_chain *pipes, int copy)
{
	int	status;

	status = 0;
	if (!pipes->pipe_count)
		ft_printf("exit\n");
	if (args[1])
	{
		if (check_argument(args[1]))
		{
			ft_printf_fd(2, "Minishell: exit: %s: ", args[1]);
			ft_printf_fd(2, "numeric argument required\n");
			exit (255);
		}
		else
			status = ft_atoi(args[1]);
	}
	if (args[1] && args[2])
	{
		ft_printf_fd(2, "Minishell: exit: too many arguments\n");
		return (1);
	}
	close(pipes->copy_stdout);
	close_pipes(pipes, copy);
	exit(status);
}
