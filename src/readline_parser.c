/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/19 17:42:52 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h" //! poista

char	*get_next_arg(char *str, int i, int len);
int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac, int rc);
int assign_cmd_pre(t_new_line *got_line);
int	assign_pointers(char *str, t_new_line *got_line, int i);
int	get_out_fd(t_cmd_pre *cmd, char *line, int i);
int	get_in_fd(t_cmd_pre *cmd, char *line, int i);
char	*make_arg_string(char *str, int len, int i);
int	count_cmd_pointers(const char *str, int *c_args, int *c_redirects);
int	read_line_parser(char *str, t_new_line *got_line);
int	create_heredoc(char *line);

int	count_substrings(char *str)
{
	int		i;
	int		count;
	char	expecting;

	i = 0;
	count = 1;
	while (str[i])
	{
		expecting = 0;
		if(str[i] == '\'' || str[i] == '\"')
		{
			expecting = skip_quotes(&str[i]);
			if (expecting < 0)
				return (expecting);
			i += expecting;
		}
		if(str[i] == '>' || str[i] == '<')
		{
			expecting = skip_redirect(&str[i], str[i], 0, 1);
			if (expecting < 0)
				return (expecting);
			i += expecting;
		}
		if (str[i] && str[i] == '|')
			count++;
		if(str[i])
			i++;
	}
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
		if (assign_pointers(str, got_line, (-1)) + assign_cmd_pre(got_line))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);;
}

int	count_cmd_pointers(const char *str, int *c_args, int *c_redirects)
{
	int	i;

	i = 0;
	*c_args = 0;
	*c_redirects = 0;
	if (!str)
		return (EXIT_FAILURE);
	while(str[i])
	{
		while(str[i] && str[i] == ' ')
			i++;
		if(str[i] && (str[i] == '<' || str[i] == '>') && ++(*c_redirects))
			i += skip_redirect(&str[i], str[i], 0, 1);
		if(str[i] && str[i] != ' ')
		{
			(*c_args)++;
			while(str[i] && str[i] != ' ')
			{
				if(str[i] && (str[i] == '\'' || str[i] == '\''))
					i += skip_quotes(&str[i]);
				i++;
			}
		}
	}	
	return (EXIT_SUCCESS);
}

char	*make_arg_string(char *str, int len, int i)
{
	int		k;
	char	*final;
	int		expecting;
	
	final = NULL;
	k = 0;
	final = malloc(sizeof(char) * (len + 1));
	if(final)
	{
		while (str[i] && k < len && str[i] != ' ')
		{
			if (str[i] == '\'' || str[i] == '\"')
			{
				if (expecting == str[i])
				{
					expecting = 0;
					i++;
					str[i] = ' ';
				}	
				else if (!expecting)
				{
					expecting = str[i];
					str[i] = ' ';
					i++;
				}
			}
			final[k++] = str[i];
			str[i++] = ' ';
		}
		final[k] = '\0';
	}
	return (final);
}

int	get_out_fd(t_cmd_pre *cmd, char *line, int i)
{
	int		out_fd;
	char	*holder;
	
	holder = NULL;
	out_fd = -2;
	while(line[i])
	{
		if(line[i] == '>')
		{
			line[i] = ' ';
			if(line[++i] == '>')
			{
				line[i] = ' ';
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY |  O_CREAT | O_APPEND , 0666);
			}
			else
			{
				
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY | O_CREAT, 0666);
			}
		}	
		if(out_fd == (-1))
		{
			free(holder);
			return (EXIT_FAILURE);
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
	char	*rline;
	int		len;

	line = get_next_arg(line, 0, 0);
	rline = NULL;
	if(line && !pipe(p))
	{
		while (1)
		{
			rline = readline(">");
			len = ft_strlen(rline);
			if(len > 1)
			{
				write(p[1], rline, len);
				write(p[1], "\n", 1);
			}
			if(!ft_strcmp(line, rline))
				break ;
			free(rline);
		}
		close(p[1]);
		free(rline);
		return(p[0]);
	}
	return (-1);
}

int	get_in_fd(t_cmd_pre *cmd, char *line, int i)
{
	int		in_fd;
	char	*holder;
	
	holder = NULL;
	in_fd = -2;
	while(line[i])
	{
		if(line[i] == '<')
		{
			line[i] = ' ';
			if(line[++i] == '<')
			{
				line[i] = ' ';
				in_fd =  create_heredoc(line);
			}
			else
			{
				holder = get_next_arg(line, 0, 0);
				in_fd = open(holder, O_RDONLY, 0666);
			}
		}	
		if(in_fd == (-1))
		{
			free(holder);
			return (EXIT_FAILURE);
		}
		i++;
	}
	cmd->in_fd = in_fd;
	free(holder);
	return (EXIT_SUCCESS);
}

char	*get_next_arg(char *str, int i, int len)
{
	char 	*final = NULL;
	int		key;
	int		start;
	int		pivot;

	len = 0;
	pivot = 0;
	while(str[i] && !pivot)
	{
		while(str[i] && str[i] == ' ')
			i++;
		if(str[i] && (str[i] == '<' || str[i] == '>'))
			i += skip_redirect(&str[i], str[i], 0, 1);
		start = i;
		while(str[i] && str[i] != ' ')
		{
			if(str[i] && (str[i] == '\'' || str[i] == '\''))
			{	
				key = str[i];
				while (str[++i])
				{
					if (str[i] == key)
						break ;
				}
			}
			len++;	
			i++;
			pivot = i;
		}
	}
	final = make_arg_string(&str[start], pivot, 0);
	return (final);
}

int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac, int rc)
{
	int i;

	i = 0;
	rc = 0;
	if(cmd->args)
		free(cmd->args);
	cmd->args = malloc(sizeof(char *) * (ac + 1));
	if(line || cmd->args)
	{
		while(i < ac)
			cmd->args[i++] = get_next_arg(line, 0, 0);
		cmd->args[i] = NULL;
		if(get_out_fd(cmd, line, 0) + get_in_fd(cmd, line, 0))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int assign_cmd_pre(t_new_line *got_line)
{
	int			i;
	int			ac;
	int			rc;

	i = 0;
	ac = 0;
	while(i < got_line->line_count)
	{
		count_cmd_pointers(got_line->exec_lines[i], &ac, &rc);
		fill_cmd_struct(got_line->exec_lines[i], &got_line->cmd_pre[i], ac, rc);
		i++;
	}
	return got_line->length;
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
	return (EXIT_SUCCESS);
}

