/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:46:21 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 12:01:51 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_env	*g_environ;

int	unset_env(const char *name)
{
	t_env	*tmp;
	t_env	*previous;
	t_env	*next;

	if (!g_environ)
		return (EXIT_SUCCESS);
	tmp = g_environ->next;
	if (!env_compare(name, g_environ->env, NO_EQUAL_SIGN))
	{
		free(g_environ);
		return (!(g_environ = tmp));
	}
	previous = g_environ;
	while (tmp->env[0])
	{
		next = tmp->next;
		if (!env_compare(name, tmp->env, NO_EQUAL_SIGN))
		{
			free(tmp);
			return (!(previous->next = next));
		}
		previous = tmp;
		tmp = next;
	}
	return (EXIT_SUCCESS);
}
