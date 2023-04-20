/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_handling_func.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 10:04:00 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/19 21:18:36 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_catcher(t_new_line *new_line, int index, int *k, int i)
{
	new_line = NULL;
	index = 0;
	k = NULL;
	i = 0;
	return (0);
}

int	word_compare(char *exec_line, char *word, int instruction)
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
	if(instruction == 1 && !exec_line[i] && !word[i])
		return (0);
	if (!exec_line[i] && !instruction)
		return (0);
	if (!instruction && exec_line[i] && (exec_line[i] == ' '
		|| exec_line[i] == '\0'))
		return (0);
	return (-1);
}

int	has_builtin(char *exec_line)
{
	int	i;

	i = 0;
	while (exec_line[i] && exec_line[i] == ' ')
		i++;
	if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "echo", 0))
		return (1);
	else if (exec_line[i] == 'c' && !word_compare(&exec_line[i], "cd", 0))
		return (2);
	else if (exec_line[i] == 'p' && !word_compare(&exec_line[i], "pwd", 0))
		return (3);
	else if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "export", 0))
		return (4);
	else if (exec_line[i] == 'u' && !word_compare(&exec_line[i], "unset", 0))
		return (5);
	else if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "env", 0))
		return (6);
	else if (exec_line[i] == 'e' && !word_compare(&exec_line[i], "exit", 0))
		return (7);
	return (-1);
}

void	line_handling_func(t_new_line *new_line)
{
	int	i;
	int k;

	i = 0;
	printf("We broke down the lines in following:\n");
	while (new_line->cmd_pre[i].args)
	{
		k = 0;
		printf("%d) lines command is: %s\n", i + 1 , new_line->cmd_pre[i].args[0]);
		printf("\tThis arguments are as followed:\n");
		while(new_line->cmd_pre[i].args[++k])
			printf("\t\tThis is the %dth argument: %s\n", k, new_line->cmd_pre[i].args[k]);
		printf("\t\tThese are the in_fd %d , and out_fd %d\n", new_line->cmd_pre[i].in_fd, new_line->cmd_pre[i].out_fd);
		
		i++;
	}
	return ;
}