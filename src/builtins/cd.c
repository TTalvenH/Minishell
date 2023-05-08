/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:45:21 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/08 10:47:30 by mkaratzi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	error(void)
{
	perror("cd");
	return (1);
}

int	cd(char	*dir)
{
	char	cwd[PATH_MAX];
	char	*path;

	if (dir && dir[0] != '/')
	{
		if (!getcwd(cwd, sizeof(cwd)))
			return (error());
		path = ft_strjoin_slash(cwd, dir);
		if (!path)
			return (error());
		if (chdir(path))
			return (error());
		return (0);
	}
	if (dir && ft_strlen(dir))
	{
		if (chdir(dir))
			return (error());
	}
	else if (!dir)
	{
		if (chdir(getenv("HOME")))
			return (error());
	}
	return (0);
}