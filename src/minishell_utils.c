#include "minishell.h"
#include "libft.h"

void	print_2d_array(char **arr) //!poista
{
	while(*arr)
	{
		ft_printf("%s\n", *arr);
		arr++;
	}
}

char	check_quotes(char *str, int i, char expecting)
{
	if (str[i] == '\'' || str[i] == '\"')
		{
			if (expecting && str[i] == expecting)
				expecting = 0;
			else if (!expecting)
				expecting = str[i];
		}
	return (expecting);
}

int	free_got_line(t_new_line *got_line)
{
	if (got_line->exec_lines)
		free(got_line->exec_lines);
	got_line->exec_lines = NULL;
	return (-1);
}
