/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/29 17:47:09 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd_pointers(const char *str, int *c_args, int *c_redirects)
{
	int	i;
	int	test;

	test = ft_strlen(str);
	i = -1;
	if (!str)
		return (EXIT_FAILURE);
	while (++i < test && str[i])
	{
		while (i < test && str[i] && str[i] == ' ')
			i++;
		if (i < test && str[i] && (str[i] == '<' || str[i] == '>')
			&& ++(*c_redirects))
			i += skip_redirect(&str[i], str[i], 0, 1);
		if (i < test && str[i] && str[i] != ' ' && ++(*c_args))
		{
			while (str[i] && str[i] != ' ')
			{
				if (str[i] && (str[i] == '\'' || str[i] == '\"'))
					skip_quotes(&str[i], &i, NULL);
				i++;
			}
		}
	}
	return (EXIT_SUCCESS);
}

int	make_arg_string_loop(int i, int fd, int *len, char *str)
{
	int	expecting;

	expecting = str[i + *len];
	str[i + (*len)++] = ' ';
	while (str[i + (*len)] && str[i + (*len)] != expecting)
	{
		write(fd, &str[i + (*len)], 1);
		str[i + (*len)++] = ' ';
	}
	return (EXIT_SUCCESS);
}

char	*make_arg_string(char *str, int len, int i)
{
	int		p[2];
	char	*final;

	final = NULL;
	if (!pipe(p))
	{
		while (str[i] && str[i] == ' ')
			i++;
		while (str[i + len] && str[i + len] != ' '
			&& str[i + len] != '<' && str[i + len] != '>')
		{	
			if ((str[i + len] == '\'' || str[i + len] == '\"'))
				make_arg_string_loop(i, p[1], &len, &str[0]);
			else if (str[i + len] == 1)
				write (p[1], "$", 1);
			else
				write (p[1], &str[i + len], 1);
			str[i + len++] = ' ';
		}
		close(p[1]);
		final = malloc(sizeof(char) * (len + 1));
		ft_bzero(final, (len + 1));
		i = read(p[0], &final[0], len) + close(p[0]);
	}
	return (final);
}

int	get_out_fd(t_cmd_pre *cmd, char *line, int i, int out_fd)
{
	char	*holder;

	holder = NULL;
	while (line[++i])
	{
		if (line[i] == '>')
		{
			line[i] = ' ';
			if (line[i + 1] == '>')
			{
				line[i + 1] = ' ';
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY | O_CREAT | O_APPEND, 0666);
			}
			else
			{
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY | O_CREAT | O_TRUNC, 0666);
			}
			break ;
		}
	}
	cmd->out_fd = out_fd;
	free(holder);
	return (EXIT_SUCCESS);
}

int	create_heredoc(char *line, int i, int len)
{
	int		p[2];
	char	*rline;

	while (line[i] && line[i] == ' ')
		i++;
	rline = NULL;
	if (!line || pipe(p))
		return (-1);
	while (word_compare(rline, &line[i], 1))
	{
		free(rline);
		rline = readline(">");
		if (rline == NULL)
		{	
			i = write(1, "\r\b\b", 1) + close(p[1]) + close(p[0]);
			return (-6);
		}
		len = ft_strlen(rline);
		if ((len && rline[0] != '\n') && word_compare(rline, &line[i], 1))
			len = write(p[1], rline, len) + write(p[1], "\n", 1);
	}
	close(p[1]);
	free(rline);
	return (p[0]);
}
