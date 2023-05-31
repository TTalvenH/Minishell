/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:39:10 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/05/31 20:08:24 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

int	exit_builtin(char *args)
{
	int	status;

	ft_printf("%s\n", args);
	status = ft_itoa(args);
	exit(status);
}
