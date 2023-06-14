/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_signals.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/14 22:07:01 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/14 22:23:23 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

struct termios	*termios_get_attr(void)
{
	struct termios	*termios_state;

	termios_state = malloc(sizeof(struct termios));
	if (tcgetattr(STDIN_FILENO, termios_state))
	{
		free(termios_state);
		return (NULL);
	}
	return (termios_state);
}

void	remove_echoctl(const struct termios *termios_state)
{
	struct termios	new_state;

	if (!termios_state)
		return ;
	if (termios_state->c_lflag & ECHOCTL)
	{
		new_state = *termios_state;
		new_state.c_lflag &= (~ECHOCTL);
		tcsetattr(STDIN_FILENO, TCSANOW, &new_state);
	}
}