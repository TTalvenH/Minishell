/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 07:27:59 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_substrings(char *str)
{
	int		i;
	int		count;
	char	expecting;

	i = 0;
	count = 1;
	expecting = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			if (expecting && str[i] == expecting)
				expecting = 0;
			else if (!expecting)
				expecting = str[i];
		}
		if (str[i] == '|' && !expecting)
			count++;
		i++;
	}
	if (expecting)
		return (-1);
	return (count);
}

int	read_line_parser(char *str, t_new_line *got_line)
{
	got_line->line_count = count_substrings(str);
	if (got_line->line_count > 0)
	{
		got_line->exec_lines = malloc(got_line->line_count * sizeof(char *));
		got_line->string = malloc((got_line->length + 1) * sizeof(char));
		if (!got_line->exec_lines || !got_line->string)
			return (free_got_line(got_line));
		assign_pointers(str, got_line);
	}	
	return (0);
}

int	assign_pointers(char *str, t_new_line *got_line)
{
	int		line;
	char	expecting;

	line = 0;
	expecting = 0;
	got_line->exec_lines[line++] = str;
	while (*str)
	{
		if (*str == '\'' || *str == '\"')
		{
			if (expecting && *str == expecting)
				expecting = 0;
			else if (!expecting)
				expecting = *str;
		}
		if (*str == '|' && !expecting)
		{
			*str = '\0';
			got_line->exec_lines[line++] = str + 1;
		}	
		*(got_line->string)++ = *str++;
	}
	*(got_line->string) = '\0';
	return (1);
}

int	free_got_line(t_new_line *got_line)
{
	free(got_line->exec_lines);
	free(got_line->string);
	return (-1);
}
