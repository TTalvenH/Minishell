/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper_funcs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 05:49:56 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/07/03 19:01:48 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	current = g_environ;
	while (current)
	{
		current = current->next;
		i++;
	}
	new_line->our_environ = malloc((i) * sizeof(char *));
	if (!new_line->our_environ)
		return (EXIT_FAILURE);
	i = 0;
	current = g_environ;
	while (current->env[0])
	{
		new_line->our_environ[i] = &(current->env[0]);
		current = current->next;
		i++;
	}
	new_line->our_environ[i] = NULL;
	return (EXIT_SUCCESS);
}

int	env_compare(const char *env1, const char *env2)
{
	int	i;

	i = 0;
	while (env1[i] && env2[i] && env1[i] != '=' && env2[i] != '='
		&& env1[i] == env2[i])
		i++;
	if (env1[i] && env2[i] && env1[i] == '=' && env2[i] == '=')
		return (EXIT_SUCCESS);
	if (env1[i] == '\0' && env2[i] == '=')
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}

void	initialise_signals(void)
{
	struct sigaction	action;

	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);
	action.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &action, NULL);
	action.sa_handler = &handler;
	sigaction(SIGINT, &action, NULL);
}
