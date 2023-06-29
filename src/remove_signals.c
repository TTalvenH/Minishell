/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 22:07:01 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/29 17:48:09 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct termios	termios_get_attr(void)
{
	struct termios	termios_state;

	ft_bzero(&termios_state, (sizeof(struct termios)));
	tcgetattr(STDIN_FILENO, &termios_state);
	return (termios_state);
}

void	remove_echoctl(const struct termios *termios_state)
{
	struct termios	new_state;

	if (termios_state->c_lflag & ECHOCTL)
	{
		new_state = *termios_state;
		new_state.c_lflag &= (~ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_state);
	}
}

int	reset_sig(const int sig)
{
	struct sigaction	act;
	struct termios		new_state;

	ft_bzero(&act, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask);
	act.sa_handler = SIG_DFL;
	act.sa_flags = 0;
	new_state = termios_get_attr();
	sigaction(SIGQUIT, &act, NULL);
	act.sa_handler = handler;
	sigaction(sig, &act, NULL);
	handler(10);
	new_state.c_lflag ^= ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_state);
	return (EXIT_SUCCESS);
}

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
	write(STDERR_FILENO, "\n", 1);
	close(STDIN_FILENO);
}
