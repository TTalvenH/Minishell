/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/25 10:33:59 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "libft.h" //! poista

char	*get_next_arg(char *str, int i, int len);
int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac, t_new_line *got_line);
int assign_cmd_pre(t_new_line *got_line);
int	assign_pointers(char *str, t_new_line *got_line, int i);
int	get_out_fd(t_cmd_pre *cmd, char *line, int i);
int	get_in_fd(t_cmd_pre *cmd, char *line, int i);
char	*make_arg_string(char *str, int len, int i);
int	count_cmd_pointers(const char *str, int *c_args, int *c_redirects);
int	read_line_parser(char *str, t_new_line *got_line);
int	create_heredoc(char *line);
int	get_cmd_fds(t_cmd_pre *cmd, char *line, int i);
int	replace_env(const char *str, int fd, char **ptrs, int *size);

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
				return (-1);
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

int write_and_count(int fd, int character, int *size)
{
	*size += write(fd, &character, 1);
	return (1);
}
char *initial_parse(const char *str, t_new_line *got_line)
{
	char	*final;
	int		p[2];
	int		i;
	int		expecting;
	int		size;

	if(pipe(p))
		return (NULL);
	i = 0;
	size = 0;
	final = NULL;
	expecting = 0;
	while(str[i])
	{
		if((str[i] == '\'' || str[i] == '\"') && !expecting)
		{
			expecting = str[i];
			i += write_and_count(p[1], str[i], &size);
		}
		else if((str[i] == '\'' || str[i] == '\"') && expecting == str[i])
		{
			expecting = 0;
			i += write_and_count(p[1], str[i], &size);
		}
		else if(str[i] == '$' && expecting != '\'')
			i += replace_env(&str[i], p[1], got_line->envs_pointers, &size);
		else
			i += write_and_count(p[1], str[i], &size);
	}
	close(p[1]);
	final = malloc(size + 1);
	final[size] = '\0';
	read(p[0], final, size);
	return (final);
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
		if (assign_pointers(got_line->parsed_line, got_line, (-1)) + assign_cmd_pre(got_line))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);;
}

int	count_cmd_pointers(const char *str, int *c_args, int *c_redirects)
{
	int	i;
	int test;

	test = ft_strlen(str);
	i = 0;
	*c_args = 0;
	*c_redirects = 0;
	if (!str)
		return (EXIT_FAILURE);
	while(i < test && str[i])
	{
		while(i < test && str[i] && str[i] == ' ')
			i++;
		if( i < test && str[i] && (str[i] == '<' || str[i] == '>') && ++(*c_redirects))
			i += skip_redirect(&str[i], str[i], 0, 1);
		if( i < test && str[i] && str[i] != ' ')
		{
			(*c_args)++;
			while(str[i] && str[i] != ' ')
			{
				if(str[i] && (str[i] == '\'' || str[i] == '\"'))
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
	int		k;
	int		p[2];
	char	*final;
	int		expecting;
	
	final = NULL;
	k = 0;
	len = 0;
	expecting = 0;
	if(!pipe(p))
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
				write(p[1], "$", 1);	
			else
				write(p[1], &str[i + len], 1);	
			str[i + len++] = ' ';
		}
		close(p[1]);
		final = malloc(sizeof(char) * (len + 1));
		ft_bzero(final, (len + 1));
		i = 0;
		while(read(p[0], &final[i], 1))
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
	while(line[i])
	{
		if(line[i] == '>')
		{
			
			line[i] = ' ';
			if(line[i + 1] == '>')
			{
				line[i + 1] = ' ';
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY |  O_CREAT | O_APPEND , 0666);
			}
			else
			{
				holder = get_next_arg(line, 0, 0);
				out_fd = open(holder, O_WRONLY | O_CREAT, 0666);
			}
			if(out_fd == (-1))
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
	while(line[i] == ' ')
		i++;
	if(line && !pipe(p))
	{
		while (1)
		{
			rline = NULL;
			rline = readline(">");
			if (rline == NULL)
			{	
				i = write(1, "\n", 1) + close(p[1]);
				free(rline);
				return (-1);
			}
			len = ft_strlen(rline);
			if(!word_compare(rline, &line[i], 1))
				break ;
			if(len > 1)
				len = write(p[1], rline, len) +	write(p[1], "\n", 1);
			free(rline);
		}
		close(p[1]);
		free(rline);
		return(p[0]);
	}
	return (-1);
}
int	get_cmd_fds(t_cmd_pre *cmd, char *line, int i)
{
	i = 0;
	
	cmd->in_fd = (-5);
	cmd->out_fd = (-5);
	cmd->stopped_heredoc = 0;
	while (line[i] && cmd->in_fd != (-2) && cmd->out_fd != (-2))
	{
		if(cmd->stopped_heredoc < 0)
			return (EXIT_FAILURE);
		if(line[i] == '>')
			get_out_fd(cmd, line, 0);
		else if(line[i] == '<')
			get_in_fd(cmd, line, 0);
		i++;
	}
	return (EXIT_SUCCESS);
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
				cmd->stopped_heredoc = in_fd;
			}
			else
			{
				holder = get_next_arg(line, 0, 0);
				in_fd = open(holder, O_RDONLY, 0666);
			}
			if(in_fd == (-1))
				in_fd = (-2);
			break ;
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
			len++;	
			pivot = i;
			i++;
		}
	}
	final = make_arg_string(&str[start], 0, 0);
	return (final);
}

int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac, t_new_line *got_line)
{
	int i;

	i = 0;
	(void)got_line;
	cmd->args = malloc(sizeof(char *) * (ac + 1));
	if(line && cmd->args)
	{
		while(i < ac)
			cmd->args[i++] = get_next_arg(line, 0, 0);
		cmd->args[i] = NULL;
		if(!get_cmd_fds(cmd, line, 0))
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	replace_env(const char *str, int fd, char **ptrs, int *size)
{
	char	*buffer;
	char	*found;
	int		k;
	int		i;

	i = ft_strlen(str);
	buffer = malloc(i + 1);
	buffer[0] = str[0];
	k = 1;
	found = NULL;
	while(str[k] && str[k] != ' ' && (ft_isalnum(str[k]) || str[k] == '_'))
	{
		buffer[k] = str[k];
		k++;
	}
	buffer[k] = '\0';
	i = -1;
	while (ptrs[++i] && !found)
		if(!env_compare(&buffer[1], ptrs[i], NO_EQUAL_SIGN))
			found = ptrs[i];
	i = 0;
	while(found != NULL && found[i] && found[i] != '=')
		i++;
	while (found != NULL && found[++i])
		write_and_count(fd, found[i], size);
	free(buffer);
	if(found == NULL && k != 1)
		return(k);
	if(found == NULL && k == 1)
		write_and_count(fd, buffer[0], size);
	return(k);
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
		if(!count_cmd_pointers(got_line->exec_lines[i], &ac, &rc) &&
			!fill_cmd_struct(got_line->exec_lines[i], &got_line->cmd_pre[i],
				ac, got_line))
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

