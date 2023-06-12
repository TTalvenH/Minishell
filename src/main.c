/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/12 18:12:36 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*g_environ;

static void	handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(STDIN_FILENO);
}

int	add_previous_result(char **str, t_new_line *got_line)
{
	char		numberstr[20];
	int			i;
	int			k;

	i = -1;
	k = 2;
	numberstr[0] = '?';
	numberstr[1] = '=';
	while (str[0] && str[0][++i])
		numberstr[k++] = str[0][i];
	numberstr[k] = '\0';
	free(str[0]);
	str[0] = NULL;
	export_env(numberstr, 1);
	ft_bzero(got_line, sizeof(t_new_line));
	llist_to_array(got_line);
	return (0);
}

static int	initialise(char *history_path, int *copy, t_new_line *got_line)
{
	signal(SIGINT, &handler);
	if (get_environments())
		return (EXIT_FAILURE);
	*copy = dup(STDIN_FILENO);
	get_history(history_path);
	ft_printf("here?\n");
	ft_bzero(got_line, sizeof(t_new_line));
	return (EXIT_SUCCESS);
}

int	main(void)
{
	char		history_path[50];
	static char	*line = NULL;
	t_new_line	got_line;
	int			copy;
	static char	*result = NULL;

	if (initialise(history_path, &copy, &got_line))
		return (EXIT_FAILURE);
	while (!dup2(copy, STDIN_FILENO))
	{
		add_previous_result(&result, &got_line);
		line = readline("Minishell: ");
		if (line == NULL && !write(0, NULL, 0))
			if (printf("\b\bexit"))
				break ;
		if (line)
		{
			got_line.length = add_to_history(line, history_path);
			if (!got_line.length || read_line_parser(line, &got_line))
				continue ;
			result = ft_itoa(piping(&got_line));
			free_got_line(&got_line, line);
		}
	}
	return (free_all_env(g_environ));
}
