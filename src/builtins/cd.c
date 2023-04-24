#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

int	cd(char	*path)
{
	char	cwd[PATH_MAX];
	char	*dir;

	dir = ft_strdup(path);
	if (!ft_strlen(dir))
		return(1);
	if (dir && dir[0] != '/')
	{
		if (!getcwd(cwd, sizeof(cwd)))
		{
			perror("cd");
			return (1);
		}
		dir = ft_realloc(dir, ft_strlen(dir) + ft_strlen(cwd) + 2);
		if (!dir)
		{
			perror("cd");
			return (1);
		}
		ft_strlcat(dir, "/", ft_strlen(dir) + 2);
		ft_strlcat(cwd, dir, ft_strlen(dir) + ft_strlen(cwd) + 1);
	}
	if (chdir(dir))
	{
		perror("cd");
		return (1);
	}
	free (dir);
	return (0);
}