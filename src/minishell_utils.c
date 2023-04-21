#include "minishell.h"
#include "libft.h"

int	close_pipes(t_pipe_chain *pipes)
{
	int i;

	i = 0;
	while (i < pipes->pipe_count)
	{
		close(pipes->pipe_fds[i][0]);
		close(pipes->pipe_fds[i][1]);
		i++;
	}
	i = 0;
	while (i < pipes->pipe_count)
	{
		free(pipes->pipe_fds[i]);
		i++;
	}
	free(pipes->pipe_fds);
	return (0);
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
	int i;

	i = 0;
	if (got_line->exec_lines)
		free(got_line->exec_lines);
	while(i < 100)
	{
		if(got_line->cmd_pre[i].args)
			free(got_line->cmd_pre[i].args);
		got_line->cmd_pre[i].args = NULL;
		if(got_line->cmd_pre[i].in_fd > 0)
			close(got_line->cmd_pre[i].in_fd);
		if(got_line->cmd_pre[i].out_fd > 0)
			close(got_line->cmd_pre[i].out_fd);
		got_line->cmd_pre[i].in_fd = (-1);
		got_line->cmd_pre[i].out_fd = (-1);
		i++;
	}
	got_line->exec_lines = NULL;
	
	return (-1);
}

int	skip_redirect(const char *str, int key, int	k, int	i)
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
	return (i);
}

int	skip_quotes(const char *str)
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
