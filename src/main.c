/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/25 07:55:10 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include "minishell.h"
#include "libft.h" //! for printf


void	handler(int sig)
{
	(void)sig;
	exit(0); //! DELETE THIS
	close(STDIN_FILENO);
}

int	main(void)
{
	char		history_path[24];
	char		*line;
	t_new_line	got_line;
	int			copy;

	signal(SIGINT, &handler);
	if(get_environments())
		return (EXIT_FAILURE);
	copy = dup(STDIN_FILENO);
	get_history(history_path);
	ft_bzero(&got_line, sizeof(t_new_line));
	while (1)
	{
		dup2(copy, STDIN_FILENO);
		line = NULL;
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
		else
			write(1, "\n", 1);
		if (got_line.exit_req == (-42))
			break ;
	}
	free(got_line.environments);
	free_all_env(g_environ);
	return (EXIT_SUCCESS);
}
