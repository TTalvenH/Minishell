/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 09:58:26 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 12:01:48 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

extern t_env	*g_environ;

int	get_environments(void)
{
	extern char	**environ;
	int			i;
	t_env		*current;

	i = 0;
	g_environ = malloc(sizeof(t_env));
	if (!g_environ)
		return (EXIT_FAILURE);
	current = g_environ;
	while (environ[i])
	{
		update_env(environ[i++], current);
		current->next = malloc(sizeof(t_env));
		if (!current->next)
			exit(!free_all_env(g_environ));
		current = current->next;
		current->env[0] = '\0';
		current->next = NULL;
	}
	return (EXIT_SUCCESS);
}

int	free_all_env(t_env *head)
{
	t_env	*current;

	while (head)
	{
		current = head->next;
		free(head);
		head = current;
	}
	return (0);
}

int	update_env(const char *env, t_env *new_env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		new_env->env[i] = env[i];
		i++;
	}
	new_env->env[i] = '\0';
	return (EXIT_SUCCESS);
}