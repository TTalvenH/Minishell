/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/20 18:10:41 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



//! TEST LINE TO SEE IF THIS GOES TO MASTER AND THEN GIT PULLING TO TEST BRANCH
#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft.h" //! for printf
int	main(void)
{
	char		history_path[24];
	char		*line;
	t_new_line	got_line;

	if(get_environments())
		return (EXIT_FAILURE);
	get_history(history_path);
	while (1)
	{
		ft_bzero(&got_line, sizeof(t_new_line));
		llist_to_array(&got_line);
		line = readline("minishell: ");
		if (line)
		{
			got_line.length = add_to_history(line, history_path);
			if(got_line.length)
			{
				read_line_parser(line, &got_line);
				// line_handling_func(&got_line);
				piping(&got_line); 
				free_got_line(&got_line);
			//here we will put the fuction for execution handling pipes, builtins , execv
			}
			free(line);
		}
		if (got_line.exit_req == (-42))
			break ;
	}
	free(got_line.environments);
	free_all_env(g_environ);
	return (EXIT_SUCCESS);
}
