/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper_funcs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 05:49:56 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/17 21:49:43 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	valid_identifier(const char *export_env)
{
	if (!export_env || (!ft_isalpha(*export_env) && *export_env != '_'))
		return (EXIT_FAILURE);
	while (*export_env && *export_env != '=')
	{
		if ((!ft_isalnum(*export_env) && *export_env != '_'))
			return (EXIT_FAILURE);
		export_env++;
	}
	return (EXIT_SUCCESS);
}

int	llist_to_array(t_new_line *new_line)
{
	t_env	*current;
	int		i;

	i = 0;
	current = new_line->environments;
	free(new_line->envs_pointers);
	current = new_line->environments;
	while(current)
	{
		current = current->next;
		i++;
	}	
	new_line->envs_pointers = malloc(i + 1);
	if(!new_line->envs_pointers)
		return (EXIT_FAILURE);
	i = 0;
	current = new_line->environments;
	while (current)
	{
		new_line->envs_pointers[i++] = current->env;
		current = current->next;
	}
	new_line->envs_pointers[i] = NULL;
	return (EXIT_SUCCESS);
}

int	env_compare(const char *env1, const char *env2)
{
	int	i;

	i = 0;
	while (env1[i] - env2[i] == 0 && env1[i] != '=' && env2[i] != '=')
		i++;
	if (env1[i] == '=' && env2[i] == '=')
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

int	update_env(const char *env1, char *env2)
{	
	int	i;

	i = 0;
	while (env1[i++] != '=')
		;
	while (env1[i] && i <= 200)
	{
		env2[i] = env1[i];
	}
	env2[i] = '\0';
	return (EXIT_SUCCESS);
}
