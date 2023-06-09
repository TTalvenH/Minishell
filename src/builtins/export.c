/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:48:55 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/06/01 18:18:55 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	our_export(char	**args, t_new_line *got_line)
{
	int	i;

	i = 0;
	if (args[i] == NULL)
		return (print_all_envs(got_line, 0));
	else
	{
		while (args[i])
		{
			export_env(args[i], 0);
			i++;
		}
	}
	return (EXIT_SUCCESS);
}

int	export_env(const char *export_env, int instruction)
{
	t_env	*head;
	t_env	holder;

	head = g_environ;
	if ((instruction == 0 && valid_identifier(export_env)))
		return (EXIT_FAILURE);
	update_env(export_env, &holder);
	while (head)
	{
		if (!env_compare(head->env, holder.env))
		{
			update_env(holder.env, head);
			ft_printf("we added %s$\n", head->env);
			return (EXIT_SUCCESS);
		}
		if (!head->next)
			break ;
		head = head->next;
	}
	update_env(holder.env, head);
	ft_printf("we added %s$\n", head->env);
	head->next = malloc(sizeof(t_env));
	if (!head->next)
		return (EXIT_FAILURE);
	ft_bzero(head->next, sizeof(t_env));
	return (EXIT_SUCCESS);
}
