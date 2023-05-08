#include "minishell.h"
#include <unistd.h>
#include <stdio.h>

static int	error()
{
	perror("pwd");
	return (1);
}

int	pwd(char *args)
{
	char cwd[PATH_MAX];

	if (args != NULL)
		error();
	if (!getcwd(cwd, sizeof(cwd)))
		return (error());
	printf("%s\n", cwd);
	return (0);	
}