/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 03:59:16 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/16 19:45:25 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*g_environ;

void	handler(int sig)
{
	static int	catcher = 0;

	if (sig != SIGINT)
	{
		catcher = sig;
		return ;
	}
	if (catcher != -42)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return ;
	}
	write(STDIN_FILENO, "\n", 1);
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
	handler(0);
	got_line->our_termios = termios_get_attr();
	llist_to_array(got_line);
	remove_echoctl(&got_line->our_termios);
	return (0);
}

static int	initialise(char *history_path, int *copy, t_new_line *got_line)
{
	struct sigaction	action;

	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	action.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &action, NULL);
	action.sa_handler = &handler;
	sigaction(SIGINT, &action, NULL);
	if (get_environments())
		return (EXIT_FAILURE);
	*copy = dup(STDIN_FILENO);
	get_history(history_path);
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
		free_got_line(&got_line, line);
		add_previous_result(&result, &got_line);
		line = readline("Minishell:");
		if (line == NULL && !(write(0, NULL, 0)))
			if (printf("\b\bexit\n"))
				break ;
		if (line)
		{
			got_line.length = add_to_history(line, history_path);
			if (!got_line.length || read_line_parser(line, &got_line))
				continue ;
			result = ft_itoa(piping(&got_line));
		}
	}
	return (free_all_env(g_environ, &line));
}
