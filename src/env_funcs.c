/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 09:58:26 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/27 09:14:57 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_environments(void)
{
	extern char	**environ;
	int			i;
	t_env		*current;

	i = 0;
	g_environ = malloc(sizeof(t_env));
	if(!g_environ)
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

int		export_env(const char *export_env)
{
	t_env	*head;
	t_env	holder;

	head = g_environ;
	if(valid_identifier(export_env) || !ft_strchr(export_env, '='))
		return (1);
	update_env(export_env, &holder);
	while (head)
	{
		if(!env_compare(head->env, holder.env, EQUAL_SIGN))
		{
			update_env(holder.env, head);
			return (EXIT_SUCCESS);	
		}
		if (!head->next)
			break ;
		head = head->next;
	}
	update_env(holder.env, head);
	head->next = malloc(sizeof(t_env));
	if(!head->next)
		return (EXIT_FAILURE);
	ft_bzero(head->next, sizeof(t_env));
	return (EXIT_SUCCESS);
}

int unset_env(const char *name)
{
	t_env	*tmp;
	t_env	*previous;
	t_env	*next;

	if(!g_environ)
		return (EXIT_SUCCESS);
	tmp = g_environ->next;
	if (!env_compare(name, g_environ->env, NO_EQUAL_SIGN))
	{
		free(g_environ);
		g_environ = tmp;
		return (EXIT_SUCCESS);
	}
	previous = g_environ;
	while(tmp->env[0])
	{
		next = tmp->next;
		if(!env_compare(name, tmp->env, NO_EQUAL_SIGN))
		{
			free(tmp);
			previous->next = next;
			return (EXIT_SUCCESS);
		}
		previous = tmp;
		tmp = next;
	}
	return(EXIT_SUCCESS);
}

int	print_all_envs(t_new_line *got_line)
{
	int i;

	i = 0;
	while(got_line->envs_pointers[i])
		ft_printf("%s\n", got_line->envs_pointers[i++]);
	return (EXIT_SUCCESS);
}

// int		unset_env(t_new_line *got_line, const char *name)
// {
// 	t_env 	*tmp;
// 	t_env	*tmp2;

// 	if(!name)
// 		return (EXIT_FAILURE);
// 	if (!env_compare(name, got_line->environments->env, NO_EQUAL_SIGN))
// 	{
// 		tmp = got_line->environments->next;
// 		free(got_line->environments);
// 		got_line->environments = tmp;
// 		return (llist_to_array(got_line));
// 	}
// 	tmp = got_line->environments;
// 	tmp2 = got_line->environments->next;
// 	while (tmp2)
// 	{
// 		if (!env_compare(name, tmp2->env, NO_EQUAL_SIGN))
// 		{
// 			tmp->next = tmp2->next;
// 			free(tmp2);
// 			tmp2 = NULL;
// 			break ;
// 		}
// 		tmp = tmp->next;
// 		tmp2 = tmp2->next;
// 	}
// 	return (llist_to_array(got_line));
// }