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
	if (got_line->cmd_pre)
		free(got_line->cmd_pre);	
	got_line->exec_lines = NULL;
	got_line->cmd_pre = NULL;
	return (-1);
}

int	skip_redirect(char *str, int key, int	k, int	i)
{
	int	found_something;

	found_something = 0;
	if (str[i] && str[i] == key)
		i++;
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i] && str[i] != ' ' && str[i] != '|'
		&& str[i] != '<' && str[i] != '>')
	{
		k = 0;
		found_something = 1;
		if (str[i] == '\'' || str[i] == '\"')
			k = skip_quotes(&str[i]);
		if (k < 0)
			return (-1);
		i += k + 1;
	}
	if (!found_something)
		return (-1);
	return (i - 1);
}

int	skip_quotes(char *str)
{
	int	i;
	int	key;

	key = str[0];
	i = 0;
	while (str[++i])
	{
		if (str[i] == key)
			return (i);
	}
	return (-1);
}
