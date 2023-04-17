/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handling_func.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 10:04:00 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/17 20:01:01 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_catcher(t_new_line *handle_line, int index, int *k, int i)
{
	handle_line = NULL;
	index = 0;
	k = NULL;
	i = 0;
	return (0);
}
int	handle_built_in(t_new_line *handle_line, int index, int k, int i)
{
	while (handle_line->exec_lines[index][i] == ' ')
		i++;
	while (handle_line->exec_lines[index][i])
	{
		while (handle_line->exec_lines[index][i] == ' ' &&
				handle_line->exec_lines[index][i] == ' ')
			i++;
		if (handle_line->exec_lines[index][i] == '\'' ||
				handle_line->exec_lines[index][i] == '\"' ||
				handle_line->exec_lines[index][i] == '<' ||
				handle_line->exec_lines[index][i] == '>')
			i += handle_catcher(handle_line, index, &k, i);
		else
			handle_line->exec_lines[index][i++] = handle_line->big_buffer[k++];

	}
	return (0);
}

int	word_compare(char *exec_line, char *word)
{
	int	i;

	i = 0;
	while (word[i])
	{
		if (exec_line[i] && word[i] == exec_line[i])
			i++;
		else
			return (-1);
	}
	if (!exec_line[i])
		return (0);
	if (exec_line[i] && (exec_line[i] == ' ' || exec_line[i] == '\0'))
		return (0);
	return (-1);
}

int	has_builtin(char *exec_line)
{
	int	i;

	i = 0;
	while (exec_line[i] && exec_line[i] == ' ')
		i++;
	if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "echo"))
		return (1);
	else if (exec_line[i] == 'c' && !word_compare(&exec_line[i], "cd"))
		return (2);
	else if (exec_line[i] == 'p' && !word_compare(&exec_line[i], "pwd"))
		return (3);
	else if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "export"))
		return (4);
	else if (exec_line[i] == 'u' && !word_compare(&exec_line[i], "unset"))
		return (5);
	else if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "env"))
		return (6);
	else if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "exit"))
		return (7);
	return (-1);
}

void	line_handling_func(t_new_line *handle_line)
{
	int	i;

	i = (-1);
	while (++i < handle_line->line_count)
	{
		if (has_builtin(handle_line->exec_lines[i]) == (-1))
			printf("This line does not have a builtin: %s\n",
				handle_line->exec_lines[i]);
		else
			printf("This line has a builtin: %s\n",
				handle_line->exec_lines[i]);
	}
	return ;
}