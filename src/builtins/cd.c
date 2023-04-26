#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

int	cd(char	*dir)
{
	char	cwd[PATH_MAX];

	if (!dir)
		dir = getenv("HOME");
	else if (dir[0] != '/')
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
		ft_strlcat(dir, cwd, ft_strlen(dir) + ft_strlen(cwd) + 1);
	}
	if (!ft_strlen(dir) && chdir(dir))
	{
		perror("cd");
		return (1);
	}
	return (0);
}