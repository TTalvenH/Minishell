/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 09:58:26 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/16 04:30:07 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_environments(t_new_line *got_line)
{
	extern char	**environ;
	int			i;
	int			k;
	t_env		*current;

	i = 0;
	k = (-1);
	got_line->environments = malloc(sizeof(t_env));
	current = got_line->environments;
	while (environ[i])
	{
		if (!current)
			exit(free_all_env(got_line->environments));
		add_env(environ[i++], current);
		if (environ[i])
		{
			current->next = malloc(sizeof(t_env));
			current = current->next;
		}
	}
	return (0);
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
	return (1);
}

int	add_env(const char *env, t_env *new_env)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while (env[i])
	{
		if (env[i] == '=')
		{
			new_env->name[i] = '\0';
			i++;
			break ;
		}
		new_env->name[i] = env[i];
		i++;
	}
	while (env[i])
	{
		new_env->value[k] = env[i];
		k++;
		i++;
	}
	new_env->value[k] = '\0';
	new_env->next = (void *)0;
	return (0);
}
