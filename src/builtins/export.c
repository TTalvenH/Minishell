/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:48:55 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 12:01:50 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern t_env	*g_environ;

int	export_env(const char *export_env)
{
	t_env	*head;
	t_env	holder;

	head = g_environ;
	if (valid_identifier(export_env) || !ft_strchr(export_env, '='))
		return (1);
	update_env(export_env, &holder);
	while (head)
	{
		if (!env_compare(head->env, holder.env, EQUAL_SIGN))
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
	if (!head->next)
		return (EXIT_FAILURE);
	ft_bzero(head->next, sizeof(t_env));
	return (EXIT_SUCCESS);
}
