/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:46:21 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/12 17:30:07 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(const char *name)
{
	t_env	*tmp;
	t_env	*previous;
	t_env	*next;

	if (!g_environ || !name)
		return (EXIT_SUCCESS);
	tmp = g_environ->next;
	if (!env_compare(name, g_environ->env))
	{
		free(g_environ);
		return (!(g_environ = tmp));
	}
	previous = g_environ;
	while (tmp->env[0])
	{
		next = tmp->next;
		if (!env_compare(name, tmp->env))
		{
			free(tmp);
			return (!(previous->next = next));
		}
		previous = tmp;
		tmp = next;
	}
	return (EXIT_SUCCESS);
}

int	unset_env(char **name)
{
	int	i;

	i = 0;
	while (name[i])
		builtin_unset(name[i++]);
	return (EXIT_SUCCESS);
}
