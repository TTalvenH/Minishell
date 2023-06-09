/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/09 18:11:12 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd_pointers(const char *str, int *c_args, int *c_redirects)
{
	int	i;
	int	test;

	test = ft_strlen(str);
	i = 0;
	*c_args = 0;
	*c_redirects = 0;
	if (!str)
		return (EXIT_FAILURE);
	while (i < test && str[i])
	{
		while (i < test && str[i] && str[i] == ' ')
			i++;
		if (i < test && str[i] && (str[i] == '<' || str[i] == '>') && ++(*c_redirects))
			i += skip_redirect(&str[i], str[i], 0, 1);
		if (i < test && str[i] && str[i] != ' ')
		{
			(*c_args)++;
			while (str[i] && str[i] != ' ')
			{
				if (str[i] && (str[i] == '\'' || str[i] == '\"'))
					i += skip_quotes(&str[i]);
				i++;
			}
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

char	*make_arg_string(char *str, int len, int i)
{
	int		p[2];
	char	*final;
	int		expecting;

	final = NULL;
	len = 0;
	expecting = 0;
	if (!pipe(p))
	{
		while (str[i] && str[i] == ' ')
			i++;
		while (str[i + len] && str[i + len] != ' ')
		{	
			if ((str[i + len] == '\'' || str[i + len] == '\"') && !expecting)
			{
				expecting = str[i + len];
				str[i + len++] = ' ';
				while (str[i + len] && str[i + len] != expecting)
				{
					write(p[1], &str[i + len], 1);
					str[i + len++] = ' ';
				}
				expecting = 0;
			}
			else if (str[i + len] == 1)
				write (p[1], "$", 1);	
			else
				write (p[1], &str[i + len], 1);	
			str[i + len++] = ' ';
		}
		close(p[1]);
		final = malloc(sizeof(char) * (len + 1));
		ft_bzero(final, (len + 1));
		i = 0;
		while (read(p[0], &final[i], 1))
			i++;
		close(p[0]);
	}
	return (final);
}

int	get_out_fd(t_cmd_pre *cmd, char *line, int i)
{
	int		out_fd;
	char	*holder;

	holder = NULL;
	out_fd = -2;
	while (line[i])
	{
		if (line[i] == '>')
		{
			line[i] = ' ';
			if (line[i + 1] == '>')
			{
				line[i + 1] = ' ';
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY |  O_CREAT | O_APPEND , 0666);
			}
			else
			{
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY | O_CREAT | O_TRUNC , 0666);
			}
			if (out_fd == (-1))
				out_fd = (-2);
			break ;
		}	
		i++;
	}
	cmd->out_fd = out_fd;
	free(holder);
	return (EXIT_SUCCESS);
}

int	create_heredoc(char *line)
{
	int		p[2];
	int		i;
	char	*rline;
	int		len;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line && !pipe(p))
	{
		while (1)
		{
			rline = NULL;
			rline = readline(">");
			if (rline == NULL)
			{	
				i = write(1, "\r\b\b", 1) + close(p[1]);
				free(rline);
				return (-6);
			}
			len = ft_strlen(rline);
			if (!word_compare(rline, &line[i], 1))
				break ;
			if (len > 1)
				len = write(p[1], rline, len) +	write(p[1], "\n", 1);
			free(rline);
		}
		close(p[1]);
		free(rline);
		return (p[0]);
	}
	return (-1);
}

int	get_in_fd(t_cmd_pre *cmd, char *line, int i)
{
	int		in_fd;
	char	*holder;

	holder = NULL;
	in_fd = -2;
	while (line[i])
	{
		if (line[i] == '<')
		{
			line[i] = ' ';
			if (line[++i] == '<')
			{
				line[i] = ' ';
				in_fd =  create_heredoc(line);
				cmd->stopped_heredoc = in_fd;
			}
			else
			{
				holder = get_next_arg(line, 0, 0);
				in_fd = open(holder, O_RDONLY, 0666);
			}
			if (in_fd == (-1))
				in_fd = (-2);
			break ;
		}
		i++;
	}
	cmd->in_fd = in_fd;
	free(holder);
	return (EXIT_SUCCESS);
}

int replace_env(const char *str, int fd, char **ptrs, int *size)
{
	char	*buffer;
	char	*found;
	int		k;
	int		i;

	if (str[1] == '?')
		return (question_mark_found(fd, size, ptrs[0])); 
	if (str[1] != '_' && !ft_isalnum(str[1]))
		return (1);
	i = ft_strlen(str);
	buffer = malloc(i + 1);
	buffer[0] = str[0];
	k = 0;
	found = NULL;
	while (str[++k] && str[k] != ' ' && (ft_isalnum(str[k]) || str[k] == '_'))
		buffer[k] = str[k];
	buffer[k] = '\0';
	i = -1;
	while (ptrs[++i] && !found)
		if (!env_compare(&buffer[1], ptrs[i]))
			found = ptrs[i];
	i = 0;
	while (found != NULL && found[i] && found[i] != '=')
		i++;
	while (found != NULL && found[++i])
		write_and_count(fd, found[i], size);
	free(buffer);
	if (found == NULL && k != 1)
		return (k);
	if (found == NULL && k == 1)
		write_and_count(fd, buffer[0], size);
	return (k);
}