/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/14 07:51:45 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char		history_path[24];
	char		*line;
	t_new_line	got_line;
	int			i;

	line = NULL;
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
				i = -1;
				while(++i < got_line.line_count)
				{
					printf("We got the %dth line: %s\n", i + 1, got_line.exec_lines[i]);
				}
			//here we will put the fuction for execution handling pipes, builtins , execv
			}	
			free(line);
		}
		
	}
	return (0);
}
