/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/18 13:26:52 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h" //! poista

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
		expecting = check_quotes(str, i, expecting);
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
		if (!got_line->exec_lines)
		{
			return (free_got_line(got_line));
		}
		assign_pointers(str, got_line, (-1));
		return (0);
	}
	return (-1);
}

int	assign_pointers(char *str, t_new_line *got_line, int i)
{
	int		line;
	char	expecting;

	line = 0;
	expecting = 0;
	got_line->exec_lines[line++] = str;
	while (str[++i])
	{
		expecting = check_quotes(str, i, expecting);
		if (str[i] == '|' && !expecting)
		{
			str[i] = '\0';
			got_line->exec_lines[line++] = &str[i + 1];
		}
	}
	return (1);
}

