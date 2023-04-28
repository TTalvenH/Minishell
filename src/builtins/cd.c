#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

static int	error()
{
	perror("cd");
	return (1);
}

// int	set_wd(char *cwd, )
int	cd(char	*dir)
{
	char	cwd[PATH_MAX];
	char	*path;
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	if (dir && dir[0] != '/')
	{
		path = ft_strjoin_slash(cwd, dir);
		if (!path)
			return (error());
		if (chdir(path))
			return (error());
		printf("%s\n", path);
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