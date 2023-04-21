/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/20 19:19:00 by mkaratzi         ###   ########.fr       */
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
int	get_cmd_fds(t_cmd_pre *cmd, char *line, int i);

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

int	read_line_parser(char *str, t_new_line *got_line)
{
	got_line->line_count = count_substrings(str);
	if (got_line->line_count > 0)
	{
		
		got_line->exec_lines = malloc(got_line->line_count * sizeof(char *));
		if (!got_line->exec_lines)
			return (free_got_line(got_line));
		if (assign_pointers(str, got_line, (-1)) + assign_cmd_pre(got_line))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	printf("we have %d\n", got_line->line_count);
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
	char	*final;
	int		expecting;
	
	final = NULL;
	k = 0;
	expecting = 0;
	len = ft_strlen(str);
	final = malloc(sizeof(char) * (500));
	if(final)
	{
		while (str[i] && i < len && str[i] == ' ')
			i++;
		while (str[i] && str[i] != ' ')
		{	
			if ((str[i] == '\'' || str[i] == '\"') && !expecting)
			{
				
				expecting = str[i];
				str[i++] = ' ';
				while (str[i] && str[i] != expecting)
				{
					if(str[i] == '$' && expecting == '\'')
						str[i] = 1;
					final[k++] = str[i];
					str[i++] = ' ';
				}
				expecting = 0;
			}
			else
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

	rline = NULL;
	i = 0;
	while(line[i] == ' ')
		i++;
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
			if(!word_compare(rline, &line[i], 1))
				break ;
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
	while (line[i] && cmd->in_fd != (-2) && cmd->out_fd != (-2))
	{
		
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

int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac, int rc)
{
	int i;

	i = 0;
	rc = 0;
	if(cmd->args)
		free(cmd->args);
	cmd->args = malloc(sizeof(char *) * (ac + 1));
	if(line && cmd->args)
	{
		while(i < ac)
			cmd->args[i++] = get_next_arg(line, 0, 0);
		cmd->args[i] = NULL;
		if(!get_cmd_fds(cmd, line, 0))
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	replace_env(const char *str, int fd, char **ptrs)
{
	char	buffer[250];
	char	*found;
	int		i;

	i = 0;
	found = NULL;
	while(str[i] && str[i] != ' ' && str[i] != '\'' && str[i] != '\"')
	{
		buffer[i] = str[i];
		i++;
	}
	buffer[i] = '\0';
	i = -1;
	while (ptrs[++i] && !found)
		if(!env_compare(&buffer[1], ptrs[i], NO_EQUAL_SIGN))
			found = ptrs[i];
	i = 0;
	if(!found)
		while(buffer[i])
			write(fd, &buffer[i++], 1);
	while(found && found[i] && found[i] != '=')
		i++;
	while (found && found[++i])
		write(fd, &found[i], 1);	
	return(ft_strlen(buffer));
}


int dollar_subs(t_cmd_pre *cmd, int count, t_new_line *got_line)
{
	int			i;
	int			k;
	int			p[2];

	i = 0;
	
	while (--count >= 0 && !pipe(p))
	{
		k = -1;
		while (cmd->args[count][++k])
		{
			if(cmd->args[count][k] == 1)
				write(p[1], "$", 1);
			else if(cmd->args[count][k] == '$')
				k = replace_env(&cmd->args[count][k], p[1], got_line->envs_pointers);
			else
				write(p[1], &cmd->args[count][k], 1);
		}
		close(p[1]);
		k = 0;
		while(read(p[0], &cmd->args[count][k], 1))
			k++;
		cmd->args[count][k] = '\0';
		close(p[0]);
	}
	return (EXIT_SUCCESS);
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
		dollar_subs(&got_line->cmd_pre[i], ac, got_line);
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

