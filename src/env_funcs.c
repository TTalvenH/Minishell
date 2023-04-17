/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 09:58:26 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/17 21:55:54 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	get_environments(t_new_line *got_line)
{
	extern char	**environ;
	int			i;
	t_env		*current;

	i = 0;
	got_line->environments = malloc(sizeof(t_env));
	current = got_line->environments;
	while (environ[i])
	{
		if (!current)
			exit(!free_all_env(got_line));
		add_env(environ[i++], current);
		if (environ[i])
		{
			current->next = malloc(sizeof(t_env));
			current = current->next;
		}
	}
	// current = got_line->environments;
	// free(got_line->envs_pointers);
	// while (current)
	// {
	// 	ft_printf("Inside 1get_envswe got: %s\n", current->env);
	// 	current = current->next;
	// }
	// exit(EXIT_SUCCESS);
	return (llist_to_array(got_line));
}

int	free_all_env(t_new_line *new_line)
{
	t_env	*current;
	t_env	*head;

	head = new_line->environments;
	while (head)
	{
		current = head->next;
		free(head);
		head = current;
	}
	free(new_line->envs_pointers);
	new_line->envs_pointers = NULL;
	return (llist_to_array(new_line));
}

int	add_env(const char *env, t_env *new_env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		new_env->env[i] = env[i];
		i++;
	}
	new_env->env[i] = '\0';
	new_env->next = (void *)0;
	return (EXIT_SUCCESS);
}

int		export_env(t_new_line *got_line, const char *export_env)
{
	t_env	*head;
	t_env	holder;
	int		found;
	
	found = 0;
	head = got_line->environments;
	if(valid_identifier(export_env) || !ft_strchr(export_env, '='))
		return (1);
	add_env(export_env, &holder);
	while (head)
	{
		if(!env_compare(head->env, holder.env))
			found = 1;
		if(!head->next || found)
			break ;
		head = head->next;
	}
	if(head && found)
		return(update_env(holder.env , head->env));
	head->next = malloc(sizeof(t_env));
	if(!head->next)
		return (1);
	add_env(export_env, head->next);
	return (llist_to_array(got_line));
}


int		unset_env(t_new_line *got_line, const char *name)
{
	t_env 	*tmp;
	t_env	*tmp2;

	if(!name)
		return (EXIT_FAILURE);
	if (!env_compare(name, got_line->environments->env))
	{
		tmp = got_line->environments->next;
		free(got_line->environments);
		got_line->environments = tmp;
		return (llist_to_array(got_line));
	}
	tmp = got_line->environments->next;
	while (tmp)
	{
		if (!env_compare(name, tmp->env))
		{
			tmp2 = tmp->next;
			free(tmp);
			tmp = tmp2;
			break ;
		}
		tmp = tmp->next;
	}
	tmp = got_line->environments;
	while(tmp)
	{
		printf("We have: %s", tmp->env);
		tmp = tmp->next;
	}
	return (0);
	return (llist_to_array(got_line));
}