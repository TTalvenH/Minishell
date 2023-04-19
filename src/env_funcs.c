/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/14 09:58:26 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/19 19:59:16 by mkaratzi         ###   ########.fr       */
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
		update_env(environ[i++], current);
		current->next = malloc(sizeof(t_env));
		if (!current->next)
			exit(!free_all_env(got_line));
		current = current->next;
		current->env[0] = '\0';
		current->next = NULL;
	}
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

int		export_env(t_new_line *got_line, const char *export_env)
{
	t_env	*head;
	t_env	holder;

	head = got_line->environments;
	if(valid_identifier(export_env) || !ft_strchr(export_env, '='))
		return (1);
	update_env(export_env, &holder);
	while (head)
	{
		if(!env_compare(head->env, holder.env, EQUAL_SIGN))
		{
			update_env(holder.env, head);
			return (llist_to_array(got_line));	
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
	return (llist_to_array(got_line));
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