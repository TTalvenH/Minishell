#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <unistd.h>
#include <signal.h>

char	*find_cmd_path(char *cmd)
{
	int			i;
	char		**paths;
	char		*cmd_path;

	i = 0;
	if (!cmd || !*cmd)
		return (NULL);
	if ((cmd[0] == '/' && !access(cmd, F_OK)))
		return (cmd);
	paths = ft_split(getenv("PATH"), ':');
	while (paths[i] != NULL)
	{
		cmd_path = ft_strjoin_slash(paths[i++], cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			ft_free_array(paths);
			return (cmd_path);
		}
		free(cmd_path);
	}
	ft_free_array(paths);
	return (NULL);
}

pid_t	child_execve(char **arg, int input_fd, int output_fd, t_pipe_chain *pipes)
{
	extern char **environ;
	pid_t	pid;
	pid = fork();
	char *cmd_path;

	cmd_path = NULL;
	if (pid == -1)
	{
		perror("fork");
		exit(0);
	}
	if (pid == 0 && (input_fd >= 0 && output_fd >= 0))
	{
		dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		close_pipes(pipes);
		cmd_path = find_cmd_path(arg[0]);
		if (cmd_path == NULL)
			ft_printf_fd(2, "Bad command\n");
		else
			execve(cmd_path, arg, environ);
		printf("test: %s\n", strerror(errno));
		exit(-1);
	}
	return (pid);
}

void	set_io_fd(t_new_line *got_line, t_pipe_chain *pipes, int i)
{
	if (!pipes->pipe_count)
	{
		pipes->in_fd = 0;
		pipes->out_fd = 1;
	}
	else if (!i)
	{
		pipes->in_fd = 0;
		pipes->out_fd = pipes->pipe_fds[i][1];
	}
	else if (i == pipes->pipe_count)
	{
		pipes->in_fd = pipes->pipe_fds[i - 1][0];
		pipes->out_fd = 1;
	}
	else
	{
		pipes->in_fd = pipes->pipe_fds[i - 1][0];
		pipes->out_fd = pipes->pipe_fds[i][1];
	}
	if (got_line->cmd_pre[i].in_fd >= 0)
		pipes->in_fd = got_line->cmd_pre[i].in_fd;
	if (got_line->cmd_pre[i].out_fd >= 0)
		pipes->out_fd = got_line->cmd_pre[i].out_fd;
}

int	init_pipes(t_pipe_chain *pipes)
{
	int i;

	i = 0;
	while (i < pipes->pipe_count)
	{
		if (pipe(pipes->pipe_fds[i++]))
			return (1);			
	}
	return (0);
}
int	piping(t_new_line *got_line)
{
	int				i;
	t_pipe_chain	pipes;

	ft_bzero(&pipes, sizeof(t_pipe_chain));
	i = 0;
	pipes.pipe_count = got_line->line_count - 1;
	if (init_pipes(&pipes))
		return (1);
	while (i <= pipes.pipe_count)
	{
		set_io_fd(got_line, &pipes, i);
		pipes.pids[i] = child_execve(got_line->cmd_pre[i].args, pipes.in_fd, pipes.out_fd, &pipes);
		i++;
	}
	if (pipes.pipe_count)
		close_pipes(&pipes);
	i = 0;
	if (pipes.pipe_count)
	{
		while(i < pipes.pipe_count + 1)
			waitpid(pipes.pids[i++], NULL, 0);
	}
	else 
		wait(0);
	return (0);
}