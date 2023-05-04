#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

static int	error()
{
	perror("cd");
	return (1);
}

static int	absolute_path(char *dir, char *old_pwd)
{
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
	if (export_env(old_pwd))
		return (error());
	return (0);
}

static int	relative_path(char *dir, char *cwd, char *old_pwd)
{
	char *path;

	path = ft_strjoin_slash(cwd, dir);
	if (!path)
		return (error());
	if (chdir(path))
		return (error());
	free (path);
	if (export_env(old_pwd))
		return (error());
	return (0);
}

int	cd(char	*dir)
{
	char	cwd[PATH_MAX];
	char	env_pwd[5];
	char	env_oldpwd[8];
	char 	*pwd;

	ft_strlcpy(env_pwd, "PWD=", 5);
	ft_strlcpy(env_oldpwd, "OLDPWD=", 8);
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	pwd = ft_strjoin(env_oldpwd, cwd);
	if (dir && dir[0] != '/')
		relative_path(dir, cwd, pwd);
	else
		absolute_path(dir, pwd);
	free (pwd);
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	pwd = ft_strjoin(env_pwd, cwd);
	export_env(pwd);
	free (pwd);
	return (0);
}