/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 16:40:07 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/09 16:54:36 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_substring_return(int count, int expecting)
{
	if (expecting >= 0)
		return (count);
	else
		return (-1);
}

int	count_substrings(char *str)
{
	int		i;
	int		count;
	char	expecting;

	i = 0;
	count = 1;
	expecting = 0;
	while (str[i] && expecting >= 0)
	{
		expecting = 0;
		if ((str[i] == '\'' || str[i] == '\"') && expecting == 0)
			expecting = skip_quotes(&str[i]);
		else if ((str[i] == '>' || str[i] == '<' ) && expecting == 0)
			expecting = skip_redirect(&str[i], str[i], 0, 1);
		i += expecting;
		if (str[i] && str[i] == '|' && expecting == 0)
			count++;
		if (str[i] && expecting >= 0)
			i++;
	}
	return (count_substring_return(count, expecting));
}

int	write_and_count(int fd, int character, int *size)
{
	*size += write(fd, &character, 1);
	return (1);
}

int	get_cmd_fds(t_cmd_pre *cmd, char *line, int i)
{
	i = 0;

	cmd->in_fd = (-5);
	cmd->out_fd = (-5);
	cmd->stopped_heredoc = 0;
	while (line[i] && cmd->in_fd != (-2) && cmd->out_fd != (-2))
	{
		if (cmd->stopped_heredoc < 0)
			return (EXIT_FAILURE);
		if (line[i] == '>')
			get_out_fd(cmd, line, 0);
		else if (line[i] == '<')
			get_in_fd(cmd, line, 0);
		if (cmd->in_fd == -6)
			return (EXIT_FAILURE);
		i++;
	}
	return (EXIT_SUCCESS);
}

char	*get_next_arg(char *str, int i, int len)
{
	char	*final = NULL;
	int		start;
	int		pivot;

	len = 0;
	pivot = 0;
	start = 0;
	while (str[i] && !pivot)
	{
		while (str[i] && str[i] == ' ')
			i++;
		if (str[i] && (str[i] == '<' || str[i] == '>'))
			i += skip_redirect(&str[i], str[i], 0, 1);
		start = i;
		while (str[i] && str[i] != ' ')
		{
			len++;	
			pivot = i;
			i++;
		}
	}
	final = make_arg_string(&str[start], 0, 0);
	return (final);
}