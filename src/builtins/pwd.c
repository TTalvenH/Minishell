/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttalvenh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 12:39:33 by ttalvenh          #+#    #+#             */
/*   Updated: 2023/05/09 12:39:36 by ttalvenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

static int	error(void)
{
	perror("pwd");
	return (1);
}

int	pwd(char *args)
{
	char	cwd[PATH_MAX];

	if (args != NULL)
		error();
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	printf("%s\n", cwd);
	return (0);
}
