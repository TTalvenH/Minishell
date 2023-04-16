/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_helper_funcs.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 05:49:56 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/04/16 06:08:28 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	valid_identifier(const char *export_env)
{
	if (!export_env || (!ft_isalpha(*export_env) && *export_env != '_'))
		return (0);
	while (*export_env && *export_env != '=')
	{
		if ((!ft_isalnum(*export_env) && *export_env != '_'))
			return (0);
		export_env++;
	}
	return (1);
}
