/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 16:55:36 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/09 19:37:31 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	assign_cmd_pre(t_new_line *got_line)
{
	int			i;
	int			ac;
	int			rc;

	i = 0;
	while (i < got_line->line_count)
	{
		rc = 0;
		ac = 0;
		if (!count_cmd_pointers(got_line->exec_lines[i], &ac, &rc)
			&& !fill_cmd_struct(got_line->exec_lines[i], &got_line->cmd_pre[i],
				ac))
			i++;
		else
			return (0);
	}
	return (got_line->length);
}

int	assign_pointers(char *str, t_new_line *got_line, int i)
{
	int		line;
	char	expecting;

	line = 0;
	expecting = 0;
	got_line->exec_lines[line] = str;
	while (str[++i])
	{
		expecting = check_quotes(str, i, expecting);
		if (str[i] == '|' && !expecting)
		{
			str[i++] = '\0';
			line++;
			got_line->exec_lines[line] = &str[i];
		}
	}
	return (EXIT_SUCCESS);
}

int	question_mark_found(int fd, int *size, const char *str)
{
	int	i;

	i = 2;
	while (str[i])
	{
		write(fd, &str[i++], 1);
		(*size)++;
	}
	return (2);
}

int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac)
{
	int	i;

	i = 0;
	cmd->args = malloc(sizeof(char *) * (ac + 1));
	if (line && cmd->args)
	{
		while (i < ac)
			cmd->args[i++] = get_next_arg(line, 0, 0);
		cmd->args[i] = NULL;
		if (!get_cmd_fds(cmd, line, 0))
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	read_line_parser(char *str, t_new_line *got_line)
{
	got_line->parsed_line = initial_parse(str, got_line);
	got_line->line_count = count_substrings(got_line->parsed_line);
	if (got_line->line_count > 0)
	{
		got_line->exec_lines = malloc(got_line->line_count * sizeof(char *));
		if (!got_line->exec_lines)
			return (free_got_line(got_line));
		assign_pointers(got_line->parsed_line, got_line, (-1));
		if (!assign_cmd_pre(got_line))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	ft_printf("Broken pipeline, please check quationmarks and redirections!\n");
	return (EXIT_FAILURE);
}
