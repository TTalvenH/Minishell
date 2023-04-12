#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>


int	main()
{
	char	*line;
	
	while(1)
	{
		line = readline("minishell: ");
		if (!line)
			return (1);
		add_history(line);
		free(line);
	}
	return (0);
}