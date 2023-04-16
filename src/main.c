/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/16 06:31:50 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	bar(int **x)
{
	int *a = malloc(sizeof(int));
	*a = 3;
	*x = a;
}
 
int	main(void)
{
	char		history_path[24];
	char		*line;
	t_new_line	got_line;

	int	a = 2;
	int *b = &a;
	bar(&b);
	printf("THIS IS GONNA BE 3 %i\n", *b);


	line = NULL;
	got_line.big_buffer = malloc(BIG_CHUNGUS);
	get_environments(&got_line);
	t_env *current;
	current = got_line.environments;
	export_env(got_line.environments, "_=");
	export_env(got_line.environments, "NOT_AGAIN");
	while(current)
	{
		// printf("We have this name:%s and value:%s\n", current->name, current->value);
		current = current->next;
	}
	get_history(history_path);
	while (1)
	{
		got_line.exec_lines = NULL;
		line = readline("minishell: ");
		if (line)
		{
			got_line.length = add_to_history(line, history_path);
			if(got_line.length)
			{
				read_line_parser(line, &got_line);
				line_handling_func(&got_line);
				free_got_line(&got_line);
			//here we will put the fuction for execution handling pipes, builtins , execv
			}
			free(line);
		}
		if (got_line.exit_req == (-42))
			break ;
	}
	free(got_line.environments);
	free(got_line.big_buffer);
	return (0);
}
