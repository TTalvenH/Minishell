/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:44:31 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 10:49:10 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_all_envs(t_new_line *got_line)
{
	int	i;

	i = 0;
	while (got_line->envs_pointers[i])
		ft_printf("%s\n", got_line->envs_pointers[i++]);
	return (EXIT_SUCCESS);
}