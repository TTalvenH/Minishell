/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   readline_parser1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/09 18:05:49 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/09 18:06:03 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	initial_parse_loop(t_nums *m_n, int fd, const char *c, t_new_line *got_line)
{
	if ((c[0] == '\'' || c[0] == '\"') && m_n->expecting == 0)
	{
		m_n->expecting = c[0];
		m_n->i += write_and_count(fd, c[0], &(m_n->size));
	}
	else if ((c[0] == '\'' || c[0] == '\"') && m_n->expecting == c[0])
	{
		m_n->expecting = c[0];
		m_n->i += write_and_count(fd, c[0], &(m_n->size));
	}
	else if (c[0] == '$' && m_n->expecting != '\'')
		m_n->i += replace_env(&c[0], fd, got_line->our_environ, &(m_n->size));
	else
		m_n->i += write_and_count(fd, c[0], &(m_n->size));
	return (EXIT_SUCCESS);
}

char	*initial_parse(const char *str, t_new_line *got_line)
{
	char	*final;
	int		p[2];
	t_nums	my_nums;

	if (pipe(p))
		return (NULL);
	my_nums.i = 0;
	my_nums.expecting = 0;
	my_nums.size = 0;
	final = NULL;
	while (str[my_nums.i])
		initial_parse_loop(&my_nums, p[1], &str[my_nums.i], got_line);
	close(p[1]);
	final = malloc(my_nums.size + 1);
	final[my_nums.size] = '\0';
	read(p[0], final, my_nums.size);
	return (final);
}