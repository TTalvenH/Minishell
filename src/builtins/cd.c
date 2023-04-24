#include "minishell.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>

int	cd(char	*dir)
{
	char	*cwd[PATH_MAX];

	if (!dir || !ft_strlen(dir))
		return(1);
	if (dir[0] != '/')
	{
		if (!getcwd(cwd, sizeof(cwd)))
		{
			perror("cd");
			return (1);
		}
		
	}
	return (0);
}