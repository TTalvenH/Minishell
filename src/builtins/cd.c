#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

static int	error()
{
	perror("cd");
	return (1);
}

static int	absolute_path(char *dir, char *cwd, char *old_pwd)
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
	if (export_env(old_pwd))
		return (error());
	printf("%s\n", path);
	return (0);
}

int	cd(char	*dir)
{
	char	cwd[PATH_MAX];
	char	env_pwd[5];
	char	env_oldpwd[8];
	char 	*old_pwd;

	ft_strlcpy(env_pwd, "PWD=", 5);
	ft_strlcpy(env_oldpwd, "OLDPWD=", 8);
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	old_pwd = ft_strjoin(env_pwd, cwd);
	if (dir && dir[0] != '/')
	{
		relative_path(dir, cwd, old_pwd);
		
	}
	else
	{
		absolute_path(dir, cwd, old_pwd);
	}
}