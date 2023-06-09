/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 09:58:26 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/01 18:08:12 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_environments(void)
{
	extern char	**environ;
	int			i;
	t_env		*current;

	i = 0;
	g_environ = malloc(sizeof(t_env));
	ft_bzero(g_environ, sizeof(t_env));
	export_env("?=0", 1);
	g_environ->next = malloc(sizeof(t_env));
	if (!g_environ)
		return (EXIT_FAILURE);
	current = g_environ->next;
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
	int					i;
	char				*holder;

	i = 0;
	holder = ft_strchr(env, '=');
	while (i <= 248 && env[i])
	{
		new_env->env[i] = env[i];
		i++;
	}
	if (holder == NULL)
	{
		if (i <= 248 )
			new_env->env[i++] = '=';
	}
	new_env->env[i] = '\0';
	return (EXIT_SUCCESS);
}
