/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 04:06:34 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/19 12:55:16 by mkaratzi         ###   ########.fr       */
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
		got_line->cmd_pre = malloc(got_line->line_count * sizeof(t_cmd_pre));
		if (!got_line->exec_lines || !got_line->cmd_pre)
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

char	*get_next_arg(char *str, int i, int len)
{
	char 	*final = NULL;
	int		key;
	int		start;
	int		check;

	len = 0;
	check = 0;
	while(str[i] && !check)
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
			check = i;
		}
	}
	ft_printf("we have this checked: %d\n", check);
	return (final);
}

int	fill_cmd_struct(char *line, t_cmd_pre *cmd, int ac, int rc)
{
	int i;

	i = 0;
	rc = 0;
	ft_printf("We got firstly here wtih ac %d\n", ac);
	if(!line)
	{
		
		cmd->args = malloc((sizeof(char *) * ac) + 1);
		ft_printf("We got here\n");
		while(i <= ac)
			cmd->args[i++] = get_next_arg(line, 0, 0);
		return (EXIT_SUCCESS);
	}
	
	return (EXIT_FAILURE);
}
int assign_cmd_pre(t_new_line *got_line)
{
	int			i;
	int			ac;
	int			rc;
	t_cmd_pre	*creation;

	i = 0;
	ac = 0;
	rc = 0;
	creation = malloc(sizeof(t_cmd_pre *));
	if(!creation)
		return (EXIT_FAILURE);
	i = 0;
	while(i < got_line->line_count)
	{
		count_cmd_pointers(got_line->exec_lines[i], &ac, &rc);
		ft_printf("This line: %s, has %d args and %d redirects\n",got_line->exec_lines[i], ac, got_line->line_count );
		fill_cmd_struct(got_line->exec_lines[i], creation, ac, rc);
		i++;
		break ;
	}
	got_line->cmd_pre = &creation;
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

