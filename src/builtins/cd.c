/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkaratzi <mkaratzi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 10:45:21 by mkaratzi          #+#    #+#             */
/*   Updated: 2023/05/09 12:38:53 by ttalvenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	error(void)
{
	perror("cd");
	return (1);
}

static int	absolute_path(char *dir, char *old_pwd, t_new_line *got_line)
{
	if (dir && ft_strlen(dir))
	{
		if (chdir(dir))
			return (error());
	}
	else if (!dir)
	{
		if (chdir(our_getenv("HOME", got_line) + 5))
			return (error());
	}
	if (export_env(old_pwd, 0))
		return (error());
	return (0);
}

static int	relative_path(char *dir, char *cwd, char *old_pwd)
{
	char	*path;

	path = ft_strjoin_slash(cwd, dir);
	if (!path)
		return (error());
	if (chdir(path))
		return (error());
	free (path);
	if (export_env(old_pwd, 0))
		return (error());
	return (0);
}

int	cd(char	*dir, t_new_line *got_line)
{
	char	cwd[PATH_MAX];
	char	env_pwd[5];
	char	env_oldpwd[8];
	char	*pwd;

	ft_strlcpy(env_pwd, "PWD=", 5);
	ft_strlcpy(env_oldpwd, "OLDPWD=", 8);
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	pwd = ft_strjoin(env_oldpwd, cwd);
	if (dir && dir[0] != '/')
		relative_path(dir, cwd, pwd);
	else
		absolute_path(dir, pwd, got_line);
	free (pwd);
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	pwd = ft_strjoin(env_pwd, cwd);
	export_env(pwd, 0);
	free (pwd);
	return (0);
}
